#pragma once

#include "pch.h"

namespace UwpEngine
{
	class CString
	{
	public:

		UINT	m_length;
		char*	m_text;

		CString();
		CString(const char* string);
		CString(UINT length);
		~CString();

		void Clear();
		void Format(const char* format, ...);
		bool Search(const char* needle);
		void Append(const char* string);
		wchar_t* MBSToWide();

	private:

		wchar_t		m_gfwcs[1024];
	};
}