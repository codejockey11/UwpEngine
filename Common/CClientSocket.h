#pragma once

#include "pch.h"

namespace UwpEngine
{
	class CClientSocket
	{
	public:

		UINT	m_clientNbr;
		SOCKET	m_socket;

		CClientSocket();
		CClientSocket(UINT clientNbr, SOCKET socket);
		~CClientSocket();
	};
}