#include "pch.h"

#include "CKeyMap.h"

UwpEngine::CKeyMap::CKeyMap()
{
	memset(this, 0x00, sizeof(CKeyMap));

	m_isDown = false;
}

UwpEngine::CKeyMap::~CKeyMap()
{
}
