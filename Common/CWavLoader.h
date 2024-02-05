#pragma once

#include "pch.h"

#include "Common/CWavFileHeader.h"
#include "Common/CString.h"

namespace UwpEngine
{
	class CWavLoader
	{
	public:

		CString*		m_wavname;
		CWavFileHeader	m_header;
		BYTE*			m_data;
		WAVEFORMATEX	m_wfx;

		CWavLoader();
		CWavLoader(const char* wavname);
		~CWavLoader();

	private:

		FILE* m_file;
	};
}