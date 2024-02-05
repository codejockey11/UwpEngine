#include "pch.h"

#include "Common/CKeyboardDevice.h"

UwpEngine::CKeyboardDevice::CKeyboardDevice()
{
	for (UINT key = 0;key < KEY_COUNT;key++)
	{
		m_keyMap[key] = new CKeyMap();
	}

	SetKeyMap((int)VirtualKey::F1, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_STARTSERVER);
	SetKeyMap((int)VirtualKey::F2, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_STOPSERVER);
	SetKeyMap((int)VirtualKey::F3, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_CONNECT);
	SetKeyMap((int)VirtualKey::F4, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_DISCONNECT);

	SetKeyMap((int)VirtualKey::F7,  KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_WINDOWMODE);
	SetKeyMap((int)VirtualKey::F8,  KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_LOADBRUSHMAP);
	SetKeyMap((int)VirtualKey::F9,  KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_LOADTERRAIN);
	SetKeyMap((int)VirtualKey::F10, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_LOADORTHOLEVEL);
	SetKeyMap((int)VirtualKey::F11, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_TOGGLECONSOLE);

	SetKeyMap((int)VirtualKey::Up,    KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_UP);
	SetKeyMap((int)VirtualKey::Left,  KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_LEFT);
	SetKeyMap((int)VirtualKey::Right, KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_RIGHT);
	SetKeyMap((int)VirtualKey::Down,  KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_MOVE_DOWN);

	SetKeyMap((int)VirtualKey::Escape, KR_ONESHOT, CPacket::PT_LOCAL, CPacket::PT_EXIT);

	SetKeyMap((int)VirtualKey::W, KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_FORWARD);
	SetKeyMap((int)VirtualKey::S, KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_BACKWARD);
	SetKeyMap((int)VirtualKey::A, KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_STRAFELEFT);
	SetKeyMap((int)VirtualKey::D, KR_REPEATER, CPacket::PT_GLOBAL, CPacket::PT_STRAFERIGHT);

	SetKeyMap((int)VirtualKey::Space, KR_ONESHOT, CPacket::PT_GLOBAL, CPacket::PT_ATTACK);
}

UwpEngine::CKeyboardDevice::~CKeyboardDevice()
{
	for (UINT key = 0; key < KEY_COUNT; key++)
	{
		delete m_keyMap[key];
	}
}

UwpEngine::CPacket* UwpEngine::CKeyboardDevice::SetState(int number)
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

void UwpEngine::CKeyboardDevice::SetKeyMap(int number, BYTE keyRepeat, BYTE audience, BYTE packetType)
{
	m_keyMap[number]->m_isDown = false;
	m_keyMap[number]->m_count = 0;
	m_keyMap[number]->m_repeater = keyRepeat;

	m_keyMap[number]->m_packet = new CPacket(0, 0, audience, packetType);
}

void UwpEngine::CKeyboardDevice::ResetKey(int number)
{
	m_keyMap[number]->m_isDown = false;
	m_keyMap[number]->m_count = 0;
}