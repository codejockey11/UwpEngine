#pragma once

#include "pch.h"

#include "Common/CClientSocket.h"
#include "Common/CLinkList.h"
#include "Common/CPacket.h"
#include "Common/CWinsockError.h"

namespace UwpEngine
{
	class CServer
	{
	public:

		CLinkList*		m_clients;
		CWinsockError*	m_winsockError;
		CPacket*		m_packet;
		SOCKET			m_listenSocket;
		int				m_client;
		BYTE			m_acceptRunning;
		BYTE			m_serverWorkerRunning;
		bool			m_isInitialized;

		CServer();
		CServer(const char* port);
		~CServer();

		bool InitializeWinsock();
		bool CreateListenSocket(const char* port);
		bool BindListenSocket();
		bool StartListenSocket();
		void Send(CPacket* packet);
		void SendNetwork(CPacket* packet, CClientSocket* clientSocket);
		void Shutdown();
		void ProcessEvent(CPacket* packet);

	private:

		struct addrinfo* m_result, * m_ptr, m_hints;

		HANDLE	m_hThread[2];
		UINT	m_listenThreadId;
		UINT	m_workerThreadId;
	};
}