#pragma once

#include "pch.h"

namespace UwpEngine
{
	class CSoundDevice
	{
	public:

		IXAudio2*				m_XAudio2;
		X3DAUDIO_HANDLE			m_X3DInstance;
		IXAudio2MasteringVoice* m_masteringVoice;
		DWORD					m_channelMask;

		CSoundDevice();
		~CSoundDevice();
	};
}