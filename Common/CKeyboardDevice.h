#pragma once

#include "pch.h"

#include "Common/CKeyMap.h"
#include "Common/CPacket.h"

namespace UwpEngine
{
	class CKeyboardDevice
	{
	public:

		enum
		{
			KR_ONESHOT = 1,
			KR_REPEATER,

			KEY_COUNT = 256,
		};

		CKeyMap*	m_keyMap[KEY_COUNT];

		CKeyboardDevice();
		~CKeyboardDevice();

		CPacket* SetState(int number);
		void SetKeyMap(int number, BYTE keyRepeat, BYTE audience, BYTE packetType);
		void ResetKey(int number);
	};
}