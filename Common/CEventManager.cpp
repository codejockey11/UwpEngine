#include "pch.h"

#include "CEventManager.h"

unsigned __stdcall ClientReceiveThread(void* c)
{
	UwpEngine::CEventManager* ec = (UwpEngine::CEventManager*)c;

	while (ec->m_isRunning)
	{
		if (ec->m_client)
		{
			if (ec->m_client->m_connectionState == UwpEngine::CClient::CS_NOTCONNECTED)
			{
				ec->m_isRunning = false;
			}
			else
			{
				ec->m_client->ClientReceive();

				if (ec->m_client->m_connectionState == UwpEngine::CClient::CS_NOTCONNECTED)
				{
					ec->m_isRunning = false;
				}
				else if (ec->m_client->m_packet->m_type)
				{
					ec->ProcessEvent(ec->m_client->m_packet);
				}
			}
		}
	}

	_endthreadex(0);

	return 0;
}

UwpEngine::CEventManager::CEventManager()
{
}

UwpEngine::CEventManager::~CEventManager()
{
	delete m_client;
	delete m_server;
}

void UwpEngine::CEventManager::SendEvent(CPacket* p)
{
	if ((m_client == nullptr) || (p->m_audience == CPacket::PT_LOCAL))
	{
		CEventManager::ProcessEvent(p);
		
		return;
	}

	m_client->SendEvent(p);
}

void UwpEngine::CEventManager::ProcessEvent(CPacket* p)
{
	switch (p->m_type)
	{
		case CPacket::PT_EXIT:
		{
			CEventManager::Exit(p);

			break;
		}

		case CPacket::PT_WINDOWMODE:
		{
			//if (globalObjects->videoDevice && globalObjects->videoDevice->tearingSupport)
			//{
				//globalObjects->videoDevice->ToggleFullscreenWindow();
			//}

			break;
		}

		case CPacket::PT_STARTSERVER:
		{
			CEventManager::StartServer();

			break;
		}

		case CPacket::PT_STOPSERVER:
		{
			if (!m_server)
			{
				//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Server Not Running\n");

				break;
			}

			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Stopping Server\n");

			m_server->Shutdown();

			break;
		}

		case CPacket::PT_CONNECT:
		{
			CEventManager::ClientConnect(p);

			break;
		}

		case CPacket::PT_PONG:
		{
			SIZE_T startTime;
			memcpy(&startTime, p->m_data, p->m_length);

			SIZE_T currTime = (SIZE_T)GetTickCount64();

			m_pingRate = currTime - startTime;

			//console->AddEventMessage("Pong %I64d %I64d", currTime, pingRate);

			break;
		}

		case CPacket::PT_DISCONNECT:
		{
			if (m_client)
			{
				if (m_client->m_connectionState != CClient::CS_CONNECTED)
				{
					//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Not Connected\n");

					break;
				}
			}
			else
			{
				break;
			}

			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Disconnect:%i\n", p->fromClient);

			m_isRunning = false;

			DWORD rv = WaitForSingleObjectEx(m_hThread, 2000, false);

			CloseHandle(m_hThread);

			m_client->Disconnect();


			CPacket* packet = new CPacket(p->m_fromClient, p->m_toClient, CPacket::PT_LOCAL, CPacket::PT_PLAYEREXIT, CPacket::PT_EMPTY, 1);

			CEventManager::SendEvent(packet);

			delete packet;

			break;
		}

		case CPacket::PT_ACCEPTED:
		{
			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Accepted:%i\n", p->fromClient);

			m_client->m_clientNbr = p->m_toClient;

			m_client->m_connectionState = CClient::CS_CONNECTED;

			break;
		}

		case CPacket::PT_CHAT:
		{
			char chatBuff[80] = {};

			sprintf_s(chatBuff, 80, "%i %s", p->m_fromClient, p->m_data);

			m_chatText.clear();

			m_chatText = chatBuff;

			//globalObjects->errorLog->WriteError("%s\n", chatText->GetText());

			//console->AddEventMessage(chatText->GetText());

			break;
		}

		case CPacket::PT_CONSOLEINPUT:
		{
			CEventManager::ConsoleInput(p);

			break;
		}

		case CPacket::PT_TOGGLECONSOLE:
		{
			if (m_consoleVisible)
			{
				m_consoleVisible = false;
			}
			else
			{
				m_consoleVisible = true;
			}

			break;
		}
	}
}

void UwpEngine::CEventManager::ConsoleInput(CPacket* p)
{
	/*
	if (p->data[0] == VK_RETURN)
	{
		if (console->commandLine->GetText()[0] != '/')
		{
			CEventManager::SendGlobalChat();

			console->Reset();

			return;
		}
		else
		{
			CEventManager::CommandFromConsole();

			console->Reset();

			return;
		}
	}

	console->AddCHAR((char)* p->data);
	*/
}

void UwpEngine::CEventManager::CommandFromConsole()
{
	/*
	if (strcmp(console->commandLine->GetText(), "/exit") == 0)
	{
		CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_EXIT, CPacket::PT_EXIT, 1);

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	if (strcmp(console->commandLine->GetText(), "/ping") == 0)
	{
		if (client == nullptr)
		{
			return;
		}

		SIZE_T startTime = (SIZE_T)GetTickCount64();

		console->AddEventMessage("Ping %I64d", startTime);

		CPacket* packet = new CPacket(client->clientNbr, client->clientNbr, CPacket::PT_SINGLE, CPacket::PT_PING, (BYTE*)&startTime, sizeof(SIZE_T));

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	if (strcmp(console->commandLine->GetText(), "/wire") == 0)
	{
		if (console->variables->drawWire)
		{
			console->variables->drawWire = false;
		}
		else
		{
			console->variables->drawWire = true;
		}

		console->AddEventMessage("drawWire %x", console->variables->drawWire);

		return;
	}

	UINT i = 0;

	char c[32] = {};

	while ((console->commandLine->GetText()[i] != ' ') && (console->commandLine->GetText()[i] != '\0'))
	{
		memcpy(&c[i], &console->commandLine->GetText()[i], 1);

		i++;
	}

	i++;

	UINT ii = 0;

	char v[32] = {};

	while (console->commandLine->GetText()[i] != '\0')
	{
		memcpy(&v[ii], &console->commandLine->GetText()[i], 1);

		ii++;
		i++;
	}

	if (strcmp(c, "/fog") == 0)
	{
		console->variables->fog = (float)atof(v);

		console->AddEventMessage("fog %f", console->variables->fog);

		return;
	}

	if (strcmp(c, "/tod") == 0)
	{
		console->variables->timeOfDay = (float)atof(v);

		console->AddEventMessage("timeOfDay %f", console->variables->timeOfDay);

		return;
	}

	if (strcmp(c, "/connect") == 0)
	{
		CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_CONNECT, v, (UINT)strlen(v));

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	if (strcmp(c, "/disconnect") == 0)
	{
		CPacket* packet = new CPacket(0, 0, CPacket::PT_LOCAL, CPacket::PT_DISCONNECT);

		CEventManager::SendEvent(packet);

		delete packet;

		return;
	}

	console->AddEventMessage("Unknown Command", c);
	*/
}

void UwpEngine::CEventManager::ClientConnect(CPacket* p)
{
	if (m_client)
	{
		if (m_client->m_connectionState == CClient::CS_CONNECTED)
		{
			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Already Connected:%i\n", p->fromClient);

			return;
		}

		delete m_client;
	}

	//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Connect\n");

	m_client = new CClient(p);

	m_isRunning = true;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, &ClientReceiveThread, (void*)this, 0, &m_recvThreadId);
}

void UwpEngine::CEventManager::Exit(CPacket* p)
{
	m_isExiting = true;

	//globalObjects->errorLog->WriteError("CEventManager::Exit:Exiting\n");

	if (m_client)
	{
		if (m_client->m_connectionState == CClient::CS_CONNECTED)
		{
			m_isRunning = false;

			DWORD rv = WaitForSingleObjectEx(m_hThread, 2000, false);

			CloseHandle(m_hThread);

			m_client->Disconnect();
		}
	}

	if (m_server)
	{
		if (m_server->m_acceptRunning)
		{
			m_server->Shutdown();
		}
	}
}

void UwpEngine::CEventManager::StartServer()
{
	if (m_server)
	{
		if (m_server->m_acceptRunning)
		{
			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Server Already Running\n");

			return;
		}

		delete m_server;
	}

	//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Starting Server\n");

	m_server = new CServer("26105");
}