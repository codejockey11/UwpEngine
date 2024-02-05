#include "pch.h"

#include "Common/CWavLoader.h"

UwpEngine::CWavLoader::CWavLoader()
{
	memset(this, 0x00, sizeof(CWavLoader));
}

UwpEngine::CWavLoader::CWavLoader(const char* wavname)
{
	memset(this, 0x00, sizeof(CWavLoader));

	errno_t error = fopen_s(&m_file, wavname, "rb");

	if (error)
	{
		return;
	}

	m_wavname = new CString(wavname);

	size_t bytesRead = fread_s(&m_header.chunkID, 4 * sizeof(BYTE), sizeof(BYTE), 4, m_file);

	bytesRead = fread_s(&m_header.chunkSize, sizeof(int), sizeof(int), 1, m_file);
	bytesRead = fread_s(&m_header.format, 4 * sizeof(BYTE), sizeof(BYTE), 4, m_file);
	bytesRead = fread_s(&m_header.subchunk1ID, 4 * sizeof(BYTE), sizeof(BYTE), 4, m_file);
	bytesRead = fread_s(&m_header.subchunk1Size, sizeof(int), sizeof(int), 1, m_file);
	bytesRead = fread_s(&m_header.audioFormat, 2 * sizeof(BYTE), sizeof(BYTE), 2, m_file);
	bytesRead = fread_s(&m_header.numChannels, 2 * sizeof(BYTE), sizeof(BYTE), 2, m_file);
	bytesRead = fread_s(&m_header.sampleRate, sizeof(int), sizeof(int), 1, m_file);
	bytesRead = fread_s(&m_header.byteRate, sizeof(int), sizeof(int), 1, m_file);
	bytesRead = fread_s(&m_header.blockAlign[2], 2 * sizeof(BYTE), sizeof(BYTE), 2, m_file);
	bytesRead = fread_s(&m_header.bitsPerSample, 2 * sizeof(BYTE), sizeof(BYTE), 2, m_file);

	if (m_header.subchunk1Size > 16)
	{
		BYTE garbage[32];
		bytesRead = fread_s(&garbage, 32, ((size_t)m_header.subchunk1Size - 16), 1, m_file);
	}

	bytesRead = fread_s(&m_header.subchunk2ID, 4 * sizeof(BYTE), sizeof(BYTE), 4, m_file);
	bytesRead = fread_s(&m_header.subchunk2Size, sizeof(int), sizeof(int), 1, m_file);

	m_data = (BYTE*)malloc(m_header.subchunk2Size);

	if (m_data)
	{
		bytesRead = fread_s(m_data, m_header.subchunk2Size * sizeof(BYTE), sizeof(BYTE), (size_t)m_header.subchunk2Size, m_file);
	}

	error = fclose(m_file);

	m_wfx.wFormatTag = m_header.audioFormat[0];
	m_wfx.nChannels = m_header.numChannels[0];
	m_wfx.nSamplesPerSec = m_header.sampleRate;
	m_wfx.nAvgBytesPerSec = m_header.byteRate;
	m_wfx.wBitsPerSample = m_header.bitsPerSample[0];
	m_wfx.nBlockAlign = (m_wfx.nChannels * m_wfx.wBitsPerSample) / 8;
	m_wfx.cbSize = 0;
}

UwpEngine::CWavLoader::~CWavLoader()
{
	delete m_wavname;

	if (m_data)
	{
		free(m_data);
	}
}