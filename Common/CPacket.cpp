#include "pch.h"

#include "Common/CPacket.h"

UwpEngine::CPacket::CPacket()
{
	memset(this, 0x00, sizeof(CPacket));
}

UwpEngine::CPacket::CPacket(int fromClient, int toClient, BYTE audience, BYTE type)
{
	memset(this, 0x00, sizeof(CPacket));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;
}

UwpEngine::CPacket::CPacket(int fromClient, int toClient, BYTE audience, BYTE type, int size, BYTE data)
{
	memset(this, 0x00, sizeof(CPacket));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;
	m_length = size;

	m_data[0] = data;
}

UwpEngine::CPacket::CPacket(int fromClient, int toClient, BYTE audience, BYTE type, int size, BYTE* data)
{
	memset(this, 0x00, sizeof(CPacket));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;
	m_length = size;

	if (m_length > DATA_LENGTH)
	{
		m_length = DATA_LENGTH;
	}

	memcpy(m_data, data, m_length);
}

UwpEngine::CPacket::~CPacket()
{
}

void UwpEngine::CPacket::Clear()
{
	memset(m_data, 0x00, DATA_LENGTH);
}