#pragma once

#include "pch.h"

#include "Common/CPacket.h"
#include "Common/CWinsockError.h"

namespace UwpEngine
{
	class CClient
	{
	public:

		CPacket*		m_packet;
		CWinsockError*	m_winsockError;
		BYTE			m_connectionState;
		SOCKET			m_connectSocket;
		int				m_clientNbr;
		char			m_hostname[132];
		char			m_ip[15];
		char			m_port[6];
		bool			m_isInitialized;

		CClient();
		CClient(CPacket* packet);
		~CClient();

		bool ConnectSocket();
		bool InitializeWinsock();
		bool CreateSocket(CPacket* packet);
		void SendEvent(CPacket* packet);
		void Disconnect();
		void ClientReceive();

		enum
		{
			CS_NOTCONNECTED = 0,
			CS_CONNECTING,
			CS_CONNECTED,
		};

	private:

		struct addrinfo* m_result, *m_ptr, m_hints;
	};
}