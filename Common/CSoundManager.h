#pragma once

#include "pch.h"

#include "Common/CSoundDevice.h"
#include "Common/CLinkList.h"
#include "Common/CSound.h"

namespace UwpEngine
{
	class CSoundManager
	{
	public:

		CSoundManager();
		CSoundManager(CSoundDevice* soundDevice);
		~CSoundManager();
		
		CSound* MakeSound(const char* soundname, bool loop);
		CSound* GetSound(const char* soundname);

	private:

		CSoundDevice*	m_soundDevice;
		CLinkList*		m_sounds;
	};
}