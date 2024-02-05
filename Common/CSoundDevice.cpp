#include "pch.h"

#include "CSoundDevice.h"

UwpEngine::CSoundDevice::CSoundDevice()
{
	memset(this, 0x00, sizeof(CSoundDevice));

	ThrowIfFailed(
		XAudio2Create(
			&m_XAudio2,
			XAUDIO2_DEBUG_ENGINE,
			XAUDIO2_DEFAULT_PROCESSOR)
	);

	ThrowIfFailed(
		m_XAudio2->CreateMasteringVoice(&m_masteringVoice)
	);

	ThrowIfFailed(
		m_masteringVoice->GetChannelMask(&m_channelMask)
	);

	ThrowIfFailed(
		X3DAudioInitialize(
			m_channelMask,
			X3DAUDIO_SPEED_OF_SOUND,
			m_X3DInstance)
	);
}

UwpEngine::CSoundDevice::~CSoundDevice()
{
	if (m_XAudio2)
	{
		m_XAudio2->Release();
	}
}