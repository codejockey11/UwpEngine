#include "pch.h"

#include "Common/CSoundManager.h"

UwpEngine::CSoundManager::CSoundManager()
{
	memset(this, 0x00, sizeof(CSoundManager));
}

UwpEngine::CSoundManager::CSoundManager(CSoundDevice* soundDevice)
{
	memset(this, 0x00, sizeof(CSoundManager));

	m_soundDevice = soundDevice;

	m_sounds = new CLinkList();
}

UwpEngine::CSoundManager::~CSoundManager()
{
	CLinkListNode* sounds = m_sounds->m_list;

	while (sounds)
	{
		CSound* sound = (CSound*)sounds->m_element;

		delete sound;

		sounds = sounds->m_next;
	}

	delete m_sounds;
}

UwpEngine::CSound* UwpEngine::CSoundManager::MakeSound(const char* soundname, bool loop)
{
	CSound* sound = GetSound(soundname);

	if (sound)
	{
		return sound;
	}

	sound = new CSound(m_soundDevice, soundname, loop);

	if (sound->m_isInitialized)
	{
		m_sounds->Add(sound, soundname);

		return sound;
	}

	throw Platform::FailureException::CreateException(E_FAIL);

	delete sound;

	return nullptr;
}

UwpEngine::CSound* UwpEngine::CSoundManager::GetSound(const char* soundname)
{
	CLinkListNode* sound = m_sounds->Search(soundname);

	if (sound)
	{
		return (CSound*)sound->m_element;
	}

	return nullptr;
}