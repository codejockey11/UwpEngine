#pragma once

#include "pch.h"

#include "Common/CPacket.h"

namespace UwpEngine
{
	class CKeyMap
	{
	public:

		bool		m_isDown;
		UINT		m_count;
		UINT		m_repeater;
		CPacket*	m_packet;

		CKeyMap();
		~CKeyMap();
	};
}