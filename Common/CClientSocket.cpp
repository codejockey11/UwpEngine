#include "pch.h"

#include "CClientSocket.h"

UwpEngine::CClientSocket::CClientSocket()
{
	memset(this, 0x00, sizeof(CClientSocket));
}

UwpEngine::CClientSocket::CClientSocket(UINT clientNbr, SOCKET socket)
{
	memset(this, 0x00, sizeof(CClientSocket));

	m_clientNbr = clientNbr;
	m_socket = socket;
}

UwpEngine::CClientSocket::~CClientSocket()
{
}