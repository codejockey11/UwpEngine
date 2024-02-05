#include "pch.h"

#include "CServer.h"

unsigned __stdcall ServerAcceptThread(void* serverObject)
{
	UwpEngine::CServer* server = (UwpEngine::CServer*)serverObject;
	
	while (server->m_acceptRunning)
	{
		if (server->m_listenSocket)
		{
			SOCKET tempSocket = accept(server->m_listenSocket, NULL, NULL);

			// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
			if ((tempSocket == INVALID_SOCKET) && (WSAGetLastError() != WSAEINTR))
			{
				closesocket(tempSocket);

				tempSocket = 0;
			}
			else if ((tempSocket == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
			{
				throw Platform::FailureException::CreateException(E_FAIL);
			}
			else
			{
				UwpEngine::CClientSocket* clientSocket = new UwpEngine::CClientSocket(server->m_client, tempSocket);

				server->m_clients->Add(clientSocket, clientSocket->m_clientNbr);

				server->m_client++;

				UwpEngine::CPacket* packet = new UwpEngine::CPacket(0, clientSocket->m_clientNbr, UwpEngine::CPacket::PT_SINGLE, UwpEngine::CPacket::PT_ACCEPTED);

				server->Send(packet);

				delete packet;
			}
		}
	}

	_endthreadex(0);

	return 0;
}

unsigned __stdcall ServerWorkerThread(void* serverObject)
{
	UwpEngine::CServer* server = (UwpEngine::CServer*)serverObject;
	
	while (server->m_serverWorkerRunning)
	{
		UwpEngine::CLinkListNode* clients = server->m_clients->m_list;

		while (clients->m_element)
		{
			UwpEngine::CClientSocket* clientSocket = (UwpEngine::CClientSocket*)clients->m_element;

			if (clients->m_deleteMe)
			{
				clients = server->m_clients->Delete(clients);
			}
			else if (clientSocket->m_socket)
			{
				UwpEngine::CPacket* packet = new UwpEngine::CPacket();

				int totalBytes = recv(clientSocket->m_socket, (char*)packet, sizeof(UwpEngine::CPacket), 0);

				if (totalBytes > 0)
				{
					server->ProcessEvent(packet);
				}
				else if (totalBytes == 0)
				{
					clients->m_deleteMe = true;

					closesocket(clientSocket->m_socket);

					clientSocket->m_socket = 0;
				}
				// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
				else if ((totalBytes == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
				{
					throw Platform::FailureException::CreateException(E_FAIL);
				}

				delete packet;

				clients = clients->m_next;
			}
		}
	}

	_endthreadex(0);

	return 0;
}

UwpEngine::CServer::CServer()
{
	memset(this, 0x00, sizeof(CServer));
}

UwpEngine::CServer::CServer(const char* port)
{
	memset(this, 0x00, sizeof(CServer));

	m_client = 1;
	m_isInitialized = false;

	m_clients = new CLinkList();

	m_winsockError = new CWinsockError();

	if (!InitializeWinsock())
	{
		return;
	}
	if (!CreateListenSocket(port))
	{
		return;
	}
	if (!BindListenSocket())
	{
		return;
	}
	if (!StartListenSocket())
	{
		return;
	}

	m_isInitialized = true;
}

UwpEngine::CServer::~CServer()
{
	CLinkListNode* clients = m_clients->m_list;

	while (clients)
	{
		CClientSocket* clientSocket = (CClientSocket*)clients->m_element;

		delete clientSocket;

		clients = clients->m_next;
	}

	delete m_clients;

	delete m_winsockError;
}

bool UwpEngine::CServer::InitializeWinsock()
{
	WORD version = MAKEWORD(2, 2);

	WSADATA wsaData;

	int error = WSAStartup(version, &wsaData);

	if (error != 0)
	{
		throw Platform::FailureException::CreateException(E_FAIL);

		return false;
	}

	return true;
}

bool UwpEngine::CServer::CreateListenSocket(const char* port)
{
	m_hints.ai_family = AF_INET;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	m_hints.ai_flags = AI_PASSIVE;

	int error = getaddrinfo(NULL, port, &m_hints, &m_result);

	if (error != 0)
	{
		throw Platform::FailureException::CreateException(E_FAIL);

		return false;
	}

	m_listenSocket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);

	if (m_listenSocket == INVALID_SOCKET)
	{
		freeaddrinfo(m_result);
		
		throw Platform::FailureException::CreateException(E_FAIL);
	}

	return true;
}

bool UwpEngine::CServer::BindListenSocket()
{
	int error = bind(m_listenSocket, m_result->ai_addr, (int)m_result->ai_addrlen);

	if (error == SOCKET_ERROR)
	{
		freeaddrinfo(m_result);

		closesocket(m_listenSocket);

		m_listenSocket = 0;

		throw Platform::FailureException::CreateException(E_FAIL);

		return false;
	}

	freeaddrinfo(m_result);

	return true;
}

bool UwpEngine::CServer::StartListenSocket()
{
	if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(m_listenSocket);

		throw Platform::FailureException::CreateException(E_FAIL);

		return false;
	}

	m_acceptRunning = true;
	m_serverWorkerRunning = true;

	m_hThread[0] = (HANDLE)_beginthreadex(NULL, 0, &ServerAcceptThread, (void*)this, 0, &m_listenThreadId);
	m_hThread[1] = (HANDLE)_beginthreadex(NULL, 0, &ServerWorkerThread, (void*)this, 0, &m_workerThreadId);

	return true;
}

void UwpEngine::CServer::Send(CPacket* packet)
{
	switch (packet->m_audience)
	{
		case CPacket::PT_SINGLE:
		{
			CLinkListNode* client = m_clients->Search(packet->m_toClient);

			if (packet->m_type == CPacket::PT_PING)
			{
				packet->m_toClient = packet->m_fromClient;
				packet->m_fromClient = 0;
				packet->m_type = CPacket::PT_PONG;
			}

			SendNetwork(packet, (CClientSocket*)client->m_element);

			break;
		}

		case CPacket::PT_GLOBAL:
		{
			CLinkListNode* clients = m_clients->m_list;

			while (clients->m_element)
			{
				SendNetwork(packet, (CClientSocket*)clients->m_element);

				clients = clients->m_next;
			}

			break;
		}
	}
}

void UwpEngine::CServer::SendNetwork(CPacket* packet, CClientSocket* clientSocket)
{
	if (!m_acceptRunning)
	{
		return;
	}

	int totalBytes = send(clientSocket->m_socket, (char*)packet, sizeof(CPacket), 0);

	if (totalBytes == SOCKET_ERROR)
	{
		throw Platform::FailureException::CreateException(E_FAIL);
	}
}

void UwpEngine::CServer::Shutdown()
{
	if (!m_acceptRunning)
	{
		return;
	}

	m_acceptRunning = false;
	m_serverWorkerRunning = false;

	DWORD result = WaitForSingleObjectEx(m_hThread[0], 2000, false);

	CloseHandle(m_hThread[0]);

	result = WaitForSingleObjectEx(m_hThread[1], 2000, false);

	CloseHandle(m_hThread[1]);

	closesocket(m_listenSocket);

	m_listenSocket = 0;

	CLinkListNode* clients = m_clients->m_list;

	while (clients->m_element)
	{
		CClientSocket* client = (CClientSocket*)clients->m_element;

		int error = shutdown(client->m_socket, SD_BOTH);

		if (error == SOCKET_ERROR)
		{
			throw Platform::FailureException::CreateException(E_FAIL);
		}

		closesocket(client->m_socket);

		client->m_socket = 0;

		clients = clients->m_next;
	}

	WSACleanup();
}

void UwpEngine::CServer::ProcessEvent(CPacket* packet)
{
	if (packet->m_type != CPacket::PT_DISCONNECT)
	{
		Send(packet);

		return;
	}

	CLinkListNode* client = m_clients->Search(packet->m_fromClient);

	if (client)
	{
		CClientSocket* socket = (CClientSocket*)client->m_element;

		closesocket(socket->m_socket);

		socket->m_socket = 0;

		client->m_deleteMe = true;
	}
}