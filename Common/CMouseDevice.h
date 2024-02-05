#pragma once

#include "pch.h"

#include "Common/CKeyMap.h"
#include "Common/CPacket.h"

namespace UwpEngine
{
	class CMouseDevice
	{
	public:

		enum
		{
			LMB = 0,
			MMB,
			RMB,
			SCR,

			KR_ONESHOT = 0,
			KR_REPEATER = 1,

			BUTTON_COUNT = 4
		};

		Platform::Agile<CoreWindow> m_window;

		float	lX;
		float	lY;
		int		lZ;

		Point m_movePos;
		Point m_prevPos;
		Point m_savePos;

		Windows::Foundation::Size m_size;

		CKeyMap* m_keyMap[BUTTON_COUNT];

		float m_sensitivity;

		CoreCursor^ m_cursor;

		CMouseDevice();
		CMouseDevice(Windows::Foundation::Size size, float sensitivity);
		~CMouseDevice();

		CPacket* SetState(UINT number);
		void SetWindow(CoreWindow^ window);
		void SetKeyMap(UINT number, bool keyRepeat, BYTE audience, BYTE packetType);
		void MouseMove();
		void MouseWheel(int delta);
		void FirstPersonReset();
		void ResetButtons();
		void SaveLastPosition();
		void SetLastPosition();

		void HideCursor() { m_cursor = m_window->PointerCursor; m_window->PointerCursor = nullptr; }
		void ShowCursor() { m_window->PointerCursor = m_cursor; }
	};
}