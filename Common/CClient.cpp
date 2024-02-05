#include "pch.h"

#include "Common/CClient.h"

UwpEngine::CClient::CClient()
{
	memset(this, 0x00, sizeof(CClient));
}

UwpEngine::CClient::CClient(CPacket* packet)
{
	memset(this, 0x00, sizeof(CClient));

	m_isInitialized = false;

	m_packet = new CPacket();

	m_winsockError = new CWinsockError();

	if (!InitializeWinsock())
	{
		return;
	}
	
	if (!CreateSocket(packet))
	{
		return;
	}

	m_isInitialized = true;
}

UwpEngine::CClient::~CClient()
{
	delete m_packet;
	delete m_winsockError;
}

bool UwpEngine::CClient::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsaData;

	int error = WSAStartup(wVersionRequested, &wsaData);

	if (error != 0)
	{
		throw Platform::FailureException::CreateException(E_FAIL);

		return false;
	}

	return true;
}

bool UwpEngine::CClient::CreateSocket(CPacket* packet)
{
	size_t i = strcspn((char*)packet->m_data, ":");

	memcpy_s(m_ip, 15, packet->m_data, i);

	i++;

	int c = 0;

	for (size_t x = i; x < i + 5; x++)
	{
		memcpy(&m_port[c], &packet->m_data[x], 1);

		c++;
	}

	m_hints.ai_family =	AF_INET;
	m_hints.ai_socktype = SOCK_STREAM;
	m_hints.ai_protocol = IPPROTO_TCP;
	m_hints.ai_flags = AI_PASSIVE;

	int error = getaddrinfo(m_ip, m_port, &m_hints, &m_result);

	if (error != 0)
	{
		throw Platform::FailureException::CreateException(E_FAIL);

		return false;
	}

	m_ptr = m_result;

	while (m_ptr != NULL)
	{
		m_connectSocket = socket(m_ptr->ai_family, m_ptr->ai_socktype, m_ptr->ai_protocol);

		if (m_connectSocket == INVALID_SOCKET)
		{
			throw Platform::FailureException::CreateException(E_FAIL);

			return false;
		}

		if (UwpEngine::CClient::ConnectSocket())
		{
			m_connectionState = CS_CONNECTING;

			break;
		}

		m_ptr = m_ptr->ai_next;
	}

	freeaddrinfo(m_result);

	gethostname(m_hostname, 132);

	return true;
}

bool UwpEngine::CClient::ConnectSocket()
{
	int error = connect(m_connectSocket, m_ptr->ai_addr, (int)m_ptr->ai_addrlen);

	if (error == SOCKET_ERROR)
	{
		closesocket(m_connectSocket);

		m_connectSocket = 0;

		throw Platform::FailureException::CreateException(E_FAIL);

		return false;
	}

	return true;
}

void UwpEngine::CClient::SendEvent(CPacket* packet)
{
	if (m_connectionState != CS_CONNECTED)
	{
		return;
	}

	int totalBytes = send(m_connectSocket, (CHAR*)packet, sizeof(CPacket), 0);

	if (totalBytes == 0)
	{
		// Server closing connection
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		throw Platform::FailureException::CreateException(E_FAIL);
	}
}

void UwpEngine::CClient::Disconnect()
{
	if (m_connectionState != CS_CONNECTED)
	{
		return;
	}

	int error = shutdown(m_connectSocket, SD_BOTH);

	if (error == SOCKET_ERROR)
	{
		throw Platform::FailureException::CreateException(E_FAIL);
	}

	m_connectionState = CS_NOTCONNECTED;

	closesocket(m_connectSocket);

	m_connectSocket = 0;

	WSACleanup();
}

void UwpEngine::CClient::ClientReceive()
{
	int totalBytes = recv(m_connectSocket, (char*)m_packet, sizeof(CPacket), 0);

	if (totalBytes == 0)
	{
		// Server closing connection

		m_connectionState = CS_NOTCONNECTED;
	}
	else if ((totalBytes == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
	{
		// WSAEINTR for some reason the hardware stopped the recv function so skipping the error check 

		throw Platform::FailureException::CreateException(E_FAIL);
	}
}