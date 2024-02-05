#pragma once

#include "pch.h"

#include "Common/CClient.h"
#include "Common/CServer.h"

namespace UwpEngine
{
	class CEventManager
	{
	public:

		CServer*	m_server;
		CClient*	m_client;

		bool		m_isRunning;
		bool		m_consoleVisible;
		bool		m_isExiting;

		CEventManager();
		~CEventManager();

		void SendEvent(CPacket* p);
		void ProcessEvent(CPacket* p);

	private:

		HANDLE			m_hThread;
		UINT			m_recvThreadId;
		std::string		m_chatText;
		size_t			m_pingRate;

		void StartServer();
		void ClientConnect(CPacket* p);
		void Exit(CPacket* p);
		void ConsoleInput(CPacket* p);
		void CommandFromConsole();
	};
}