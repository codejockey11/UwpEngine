#pragma once

#include "pch.h"

#include "Common/CSoundDevice.h"
#include "Common/CString.h"
#include "Common/CWavLoader.h"

namespace UwpEngine
{
	class CSound
	{
	public:

		enum
		{
			SE_REVERB = 0,
			SE_ECHO,
			SE_REVERBXAPO,

			EFFECT_COUNT = 3,

			SOURCE_VOICE_COUNT = 16
		};

		CString*	m_soundname;
		bool		m_isInitialized;

		CSound();
		CSound(CSoundDevice* soundDevice, const char* soundname, bool loop);
		~CSound();
		
		void ToggleEffect(int effect);
		void StartSound();
		void StopSound();
		void Shutdown();

	private:

		CSoundDevice*			m_soundDevice;
		CWavLoader*				m_wavLoader;
		XAUDIO2_EFFECT_CHAIN	m_effectChain;
		IXAudio2SourceVoice*	m_sourceVoice[SOURCE_VOICE_COUNT];
		XAUDIO2_BUFFER			m_buffer;
		IUnknown*				m_xapoEffect[EFFECT_COUNT];
		bool					m_isOn[EFFECT_COUNT];
	};
}