#include "pch.h"

#include "CMouseDevice.h"

UwpEngine::CMouseDevice::CMouseDevice()
{
	memset(this, 0x00, sizeof(CMouseDevice));
}

UwpEngine::CMouseDevice::CMouseDevice(Windows::Foundation::Size size, float sensitivity)
{
	memset(this, 0x00, sizeof(CMouseDevice));

	m_size = size;
	m_sensitivity = sensitivity;

	for (UINT key = 0;key < BUTTON_COUNT;key++)
	{
		m_keyMap[key] = new CKeyMap();
	}

	SetKeyMap(LMB, KR_ONESHOT, CPacket::PT_GLOBAL, CPacket::PT_ATTACK);
	SetKeyMap(MMB, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_EXIT);
	SetKeyMap(RMB, KR_REPEATER, CPacket::PT_LOCAL, CPacket::PT_FPSMOVE);
}

UwpEngine::CMouseDevice::~CMouseDevice()
{
	for (UINT k = 0; k < BUTTON_COUNT; k++)
	{
		delete m_keyMap[k];
	}
}

void UwpEngine::CMouseDevice::SetWindow(CoreWindow^ window)
{
	m_window = window;

	m_window->PointerPosition = Point(m_window->Bounds.Right - m_window->Bounds.Left / 2, m_window->Bounds.Bottom - m_window->Bounds.Top / 2);
	
	m_prevPos.X = m_window->PointerPosition.X;
	m_prevPos.Y = m_window->PointerPosition.Y;

	m_cursor = m_window->PointerCursor;
}

UwpEngine::CPacket* UwpEngine::CMouseDevice::SetState(UINT number)
{
	m_keyMap[number]->m_count++;

	if (m_keyMap[number]->m_count > 1)
	{
		m_keyMap[number]->m_count = 2;
	}

	if ((m_keyMap[number]->m_count == 1) && (m_keyMap[number]->m_repeater == KR_ONESHOT))
	{
		return m_keyMap[number]->m_packet;
	}
	else if ((m_keyMap[number]->m_count >= 1) && (m_keyMap[number]->m_repeater == KR_REPEATER))
	{
		return m_keyMap[number]->m_packet;
	}

	return nullptr;
}

void UwpEngine::CMouseDevice::SetKeyMap(UINT number, bool keyRepeat, BYTE audience, BYTE packetType)
{
	m_keyMap[number]->m_isDown = false;
	m_keyMap[number]->m_count = 0;
	m_keyMap[number]->m_repeater = keyRepeat;

	m_keyMap[number]->m_packet = new CPacket(0, 0, audience, packetType);
}

void UwpEngine::CMouseDevice::MouseMove()
{
	m_movePos.X = m_window->PointerPosition.X;
	m_movePos.Y = m_window->PointerPosition.Y;

	lX = m_movePos.X - m_prevPos.X;
	lY = m_movePos.Y - m_prevPos.Y;
}

void UwpEngine::CMouseDevice::MouseWheel(int d)
{
	lZ = d;
}

void UwpEngine::CMouseDevice::FirstPersonReset()
{
	lX = 0;
	lY = 0;
	lZ = 0;

	m_window->PointerPosition = Point(m_window->Bounds.Right / 2, m_window->Bounds.Bottom / 2);

	m_prevPos.X = m_window->PointerPosition.X;
	m_prevPos.Y = m_window->PointerPosition.Y;
}

void UwpEngine::CMouseDevice::ResetButtons()
{
	m_keyMap[LMB]->m_isDown = false;
	m_keyMap[LMB]->m_count = 0;
	
	m_keyMap[MMB]->m_isDown = false;
	m_keyMap[MMB]->m_count = 0;
	
	m_keyMap[RMB]->m_isDown = false;
	m_keyMap[RMB]->m_count = 0;
}

void UwpEngine::CMouseDevice::SaveLastPosition()
{
	m_savePos.X = m_window->PointerPosition.X;
	m_savePos.Y = m_window->PointerPosition.Y;
}

void UwpEngine::CMouseDevice::SetLastPosition()
{
	m_window->PointerPosition = Point(m_savePos.X, m_savePos.Y);
}