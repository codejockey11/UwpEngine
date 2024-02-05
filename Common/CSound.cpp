#include "pch.h"

#include "Common/CSound.h"

UwpEngine::CSound::CSound()
{
}

UwpEngine::CSound::CSound(CSoundDevice* soundDevice, const char* soundname, bool loop)
{
	m_soundDevice = soundDevice;
	m_soundname = new CString(soundname);
	m_isInitialized = false;

	m_wavLoader = new CWavLoader(m_soundname->m_text);

	if (m_wavLoader->m_data == 0)
	{
		return;
	}

	for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
	{
		ThrowIfFailed(
			m_soundDevice->m_XAudio2->CreateSourceVoice(
				&m_sourceVoice[i],
				&m_wavLoader->m_wfx)
		);
	}

	m_buffer.AudioBytes = m_wavLoader->m_header.subchunk2Size;
	m_buffer.Flags = XAUDIO2_END_OF_STREAM;
	m_buffer.pAudioData = m_wavLoader->m_data;

	if (loop)
	{
		m_buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// Princeton Digital Reverb
	ThrowIfFailed(
		XAudio2CreateReverb(&m_xapoEffect[SE_REVERB])
	);

	ThrowIfFailed(
		CreateFX(__uuidof(FXEcho), &m_xapoEffect[SE_ECHO])
	);

	ThrowIfFailed(
		CreateFX(__uuidof(FXReverb), &m_xapoEffect[SE_REVERBXAPO])
	);

	FXREVERB_PARAMETERS xapoReverb;

	xapoReverb.Diffusion = FXREVERB_DEFAULT_DIFFUSION;
	xapoReverb.RoomSize = FXREVERB_DEFAULT_ROOMSIZE;

	FXECHO_PARAMETERS xapoEcho;

	xapoEcho.Delay = 250.0F;	// FXECHO_DEFAULT_DELAY;
	xapoEcho.Feedback = 0.50f;	// FXECHO_DEFAULT_FEEDBACK;
	xapoEcho.WetDryMix = 0.50f;	// FXECHO_DEFAULT_WETDRYMIX;

	XAUDIO2FX_REVERB_PARAMETERS	reverbParameters;

	reverbParameters.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
	reverbParameters.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
	reverbParameters.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
	reverbParameters.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	reverbParameters.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	reverbParameters.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	reverbParameters.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	reverbParameters.EarlyDiffusion = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION;
	reverbParameters.LateDiffusion = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION;
	reverbParameters.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
	reverbParameters.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
	reverbParameters.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
	reverbParameters.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
	reverbParameters.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
	reverbParameters.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
	reverbParameters.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
	reverbParameters.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
	reverbParameters.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
	reverbParameters.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
	reverbParameters.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
	reverbParameters.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
	reverbParameters.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;

	//XAUDIO2FX_REVERB_I3DL2_PARAMETERS ppp = XAUDIO2FX_I3DL2_PRESET_STONEROOM;
	//ReverbConvertI3DL2ToNative(&ppp, &reverbParameters);

	XAUDIO2_EFFECT_DESCRIPTOR effectDesc[EFFECT_COUNT];

	effectDesc[SE_REVERB].InitialState = false;
	effectDesc[SE_REVERB].OutputChannels = m_wavLoader->m_wfx.nChannels;
	effectDesc[SE_REVERB].pEffect = m_xapoEffect[SE_REVERB];

	effectDesc[SE_ECHO].InitialState = false;
	effectDesc[SE_ECHO].OutputChannels = m_wavLoader->m_wfx.nChannels;
	effectDesc[SE_ECHO].pEffect = m_xapoEffect[SE_ECHO];

	effectDesc[SE_REVERBXAPO].InitialState = false;
	effectDesc[SE_REVERBXAPO].OutputChannels = m_wavLoader->m_wfx.nChannels;
	effectDesc[SE_REVERBXAPO].pEffect = m_xapoEffect[SE_REVERBXAPO];


	m_effectChain.EffectCount = EFFECT_COUNT;
	m_effectChain.pEffectDescriptors = effectDesc;

	for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
	{
		ThrowIfFailed(
			m_sourceVoice[i]->SetEffectChain(&m_effectChain)
		);
	}

	for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
	{
		ThrowIfFailed(
			m_sourceVoice[i]->SetEffectParameters(
				SE_REVERB,
				&reverbParameters,
				sizeof(XAUDIO2FX_REVERB_PARAMETERS),
				XAUDIO2_COMMIT_NOW)
		);

		ThrowIfFailed(
			m_sourceVoice[i]->SetEffectParameters(
				SE_ECHO,
				&xapoEcho,
				sizeof(FXECHO_PARAMETERS),
				XAUDIO2_COMMIT_NOW)
		);

		ThrowIfFailed(
			m_sourceVoice[i]->SetEffectParameters(
				SE_REVERBXAPO,
				&xapoReverb,
				sizeof(FXREVERB_PARAMETERS),
				XAUDIO2_COMMIT_NOW)
		);
	}

	//ToggleEffect(SE_ECHO);
	//ToggleEffect(SE_REVERB);
	//ToggleEffect(SE_REVERBXAPO);

	m_isInitialized = true;
}

UwpEngine::CSound::~CSound()
{
	delete m_soundname;
	delete m_wavLoader;

	if (m_xapoEffect[SE_REVERB])
	{
		m_xapoEffect[SE_REVERB]->Release();
	}
	
	if (m_xapoEffect[SE_ECHO])
	{
		m_xapoEffect[SE_ECHO]->Release();
	}

	if (m_xapoEffect[SE_REVERBXAPO])
	{
		m_xapoEffect[SE_REVERBXAPO]->Release();
	}
}

void UwpEngine::CSound::Shutdown()
{
	for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
	{
		ThrowIfFailed(
			m_sourceVoice[i]->FlushSourceBuffers()
		);

		m_sourceVoice[i]->DestroyVoice();
	}
}

void UwpEngine::CSound::ToggleEffect(int effect)
{
	if (m_isOn[effect])
	{
		m_isOn[effect] = false;

		for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
		{
			ThrowIfFailed(
				m_sourceVoice[i]->DisableEffect(effect)
			);
		}
	}
	else
	{
		m_isOn[effect] = true;

		for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
		{
			ThrowIfFailed(
				m_sourceVoice[i]->EnableEffect(effect)
			);
		}
	}
}

void UwpEngine::CSound::StartSound()
{
	XAUDIO2_VOICE_STATE vs;
	
	for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
	{
		m_sourceVoice[i]->GetState(&vs);

		if (vs.BuffersQueued == 1)
		{
		}
		else
		{
			ThrowIfFailed(
				m_sourceVoice[i]->SubmitSourceBuffer(&m_buffer)
			);

			ThrowIfFailed(
				m_sourceVoice[i]->Start(0)
			);

			break;
		}
	}
}

void UwpEngine::CSound::StopSound()
{
	for (UINT i = 0; i < SOURCE_VOICE_COUNT; i++)
	{
		ThrowIfFailed(
			m_sourceVoice[i]->Stop(0)
		);
	}
}