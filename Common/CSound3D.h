#pragma once

#include "pch.h"

#include "Common/CSoundDevice.h"
#include "Common/CString.h"
#include "Common/CWavLoader.h"

namespace UwpEngine
{
	class CSound3D
	{
	public:

		CString*				m_soundname;
		X3DAUDIO_LISTENER		m_listener;
		X3DAUDIO_EMITTER		m_emitter;
		X3DAUDIO_CONE			m_emitterCone;
		X3DAUDIO_DSP_SETTINGS	m_dspSettings;
		X3DAUDIO_VECTOR			m_listenerPos;
		X3DAUDIO_VECTOR			m_emitterPos;
		float					m_listenerAngle;

		CSound3D();
		CSound3D(CSoundDevice* soundDevice, const char* soundname, bool loop, X3DAUDIO_VECTOR* position, float range);
		~CSound3D();

		void SetListener(X3DAUDIO_VECTOR* position, X3DAUDIO_VECTOR* listener, X3DAUDIO_VECTOR* listenerUp);
		void StartSound();
		void StopSound();
		void Shutdown();

	private:

		CSoundDevice*			m_soundDevice;
		CWavLoader*				m_wavLoader;
		float					m_range;
		IXAudio2SourceVoice*	m_sourceVoice;
		XAUDIO2_BUFFER			m_buffer;
		FLOAT32*				m_matrix;
		DWORD					m_dwCalcFlags;
		X3DAUDIO_VECTOR			m_distance;
		int						m_count;
	};
}