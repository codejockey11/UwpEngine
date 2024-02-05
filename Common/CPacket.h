#pragma once

#include "pch.h"

namespace UwpEngine
{
	class CPacket
	{
	public:

		enum
		{
			DATA_LENGTH = 32
		};

		int		m_fromClient;
		int		m_toClient;
		BYTE	m_audience;
		BYTE	m_type;
		int		m_length;
		BYTE	m_data[DATA_LENGTH];

		CPacket();
		CPacket(int fromClient, int toClient, BYTE audience, BYTE type);
		CPacket(int fromClient, int toClient, BYTE audience, BYTE type, int size, BYTE data);
		CPacket(int fromClient, int toClient, BYTE audience, BYTE type, int size, BYTE* data);
		~CPacket();

		void Clear();

		enum
		{
			PT_EMPTY = 0,

			PT_PLAYERORTHOENTER,
			PT_PLAYERORTHOEXIT,

			PT_PLAYERENTER,
			PT_PLAYEREXIT,

			PT_CONNECT,
			PT_DISCONNECT,

			PT_STARTSERVER,
			PT_STOPSERVER,
			PT_ACCEPTED,

			PT_PING,
			PT_PONG,

			PT_TOGGLECONSOLE,
			PT_CONSOLEPAGING,
			PT_CONSOLEINPUT,

			PT_LOADBRUSHMAP,
			PT_LOADTERRAIN,
			PT_LOADORTHOLEVEL,

			PT_CHAT,

			PT_SINGLE,
			PT_TEAM,
			PT_GLOBAL,
			PT_LOCAL,

			PT_MOVE_DOWN,
			PT_MOVE_UP,
			PT_MOVE_LEFT,
			PT_MOVE_RIGHT,

			PT_FORWARD,
			PT_BACKWARD,
			PT_STRAFELEFT,
			PT_STRAFERIGHT,

			PT_ATTACK,

			PT_RENDERFRAME,
			PT_WINDOWMODE,

			PT_FPSMOVE,
			PT_MMOMOVE,

			PT_DEFAULT,

			PT_EXIT
		};
	};
}