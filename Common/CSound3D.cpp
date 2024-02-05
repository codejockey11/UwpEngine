#include "pch.h"

#include "CSound3D.h"

UwpEngine::CSound3D::CSound3D()
{
	memset(this, 0x00, sizeof(CSound3D));
}

UwpEngine::CSound3D::CSound3D(CSoundDevice* soundDevice, const char* soundname, bool loop, X3DAUDIO_VECTOR* position, float range)
{
	memset(this, 0x00, sizeof(CSound3D));

	m_soundDevice = soundDevice;
	m_soundname = new CString(soundname);
	m_range = range;
		
	m_wavLoader = new CWavLoader(m_soundname->m_text);

	ThrowIfFailed(
		m_soundDevice->m_XAudio2->CreateSourceVoice(
			&m_sourceVoice,
			&m_wavLoader->m_wfx)
	);

	m_buffer.AudioBytes = m_wavLoader->m_header.subchunk2Size;
	m_buffer.Flags = XAUDIO2_END_OF_STREAM;
	m_buffer.pAudioData = m_wavLoader->m_data;

	if (loop)
	{
		m_buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	m_emitter.Position.x = position->x;
	m_emitter.Position.y = position->y;
	m_emitter.Position.z = position->z;

	m_emitter.ChannelCount = 1;
	m_emitter.CurveDistanceScaler = 1.0f;

	m_dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX;

	// deviceDetails overly complicated just use 2 channels
	m_matrix = new FLOAT32[2];
	
	memset(m_matrix, 0x00, 2 * sizeof(FLOAT32));
	
	m_dspSettings.SrcChannelCount = m_wavLoader->m_wfx.nChannels;
	m_dspSettings.DstChannelCount = 2;
	m_dspSettings.pMatrixCoefficients = m_matrix;
}

UwpEngine::CSound3D::~CSound3D()
{
	delete m_soundname;
	delete m_wavLoader;

	m_sourceVoice->Stop(0);
	m_sourceVoice->FlushSourceBuffers();
	m_sourceVoice->DestroyVoice();
}

void UwpEngine::CSound3D::SetListener(X3DAUDIO_VECTOR* position, X3DAUDIO_VECTOR* listener, X3DAUDIO_VECTOR* listenerUp)
{
	memset(&m_listener, 0x00, sizeof(X3DAUDIO_LISTENER));

	m_listener.Position = *position;
	m_listener.OrientFront = *listener;
	m_listener.OrientTop = *listenerUp;

	m_distance.x = m_listener.Position.x - m_emitter.Position.x;
	m_distance.y = m_listener.Position.y - m_emitter.Position.y;
	m_distance.z = m_listener.Position.z - m_emitter.Position.z;
	
	XMVECTOR res = XMVector3Length(XMLoadFloat3(&m_distance));

	if (XMVectorGetX(res) > m_range)
	{
		m_count = 0;
		
		StopSound();
	}
	else if (m_count == 0)
	{
		m_count = 1;
		
		StartSound();
	}

	X3DAudioCalculate(m_soundDevice->m_X3DInstance, &m_listener, &m_emitter, m_dwCalcFlags, &m_dspSettings);

	// just going to use 2 channels
	m_sourceVoice->SetOutputMatrix(m_soundDevice->m_masteringVoice, m_wavLoader->m_wfx.nChannels, 2, m_dspSettings.pMatrixCoefficients, 0);
}

void UwpEngine::CSound3D::Shutdown()
{
	m_sourceVoice->FlushSourceBuffers();
	m_sourceVoice->DestroyVoice();
}

void UwpEngine::CSound3D::StartSound()
{
	ThrowIfFailed(
		m_sourceVoice->SubmitSourceBuffer(&m_buffer)
	);

	ThrowIfFailed(
		m_sourceVoice->Start(0)
	);
}

void UwpEngine::CSound3D::StopSound()
{
	ThrowIfFailed(
		m_sourceVoice->Stop(0)
	);
}