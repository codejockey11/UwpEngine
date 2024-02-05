#include "pch.h"

#include "CString.h"

UwpEngine::CString::CString()
{
	memset(this, 0x00, sizeof(CString));
}

UwpEngine::CString::CString(const char* string)
{
	memset(this, 0x00, sizeof(CString));

	if (string == NULL)
	{
		return;
	}

	m_length = (UINT)strlen(string);

	m_text = new char[m_length + 1];

	memset(m_text, 0x00, m_length + 1);

	memcpy(m_text, string, m_length);
}

UwpEngine::CString::CString(UINT length)
{
	memset(this, 0x00, sizeof(CString));

	m_length = length;

	m_text = new char[length + 1];

	memset(m_text, 0x00, length + 1);
}

UwpEngine::CString::~CString()
{
	delete[] m_text;
}

void UwpEngine::CString::Clear()
{
	memset(m_text, 0x00, m_length);
}

void UwpEngine::CString::Format(const char* format, ...)
{
	va_list	argptr;

	va_start(argptr, format);

	Clear();

	vsprintf_s(m_text, m_length, format, argptr);

	va_end(argptr);
}

bool UwpEngine::CString::Search(const char* needle)
{
	std::string s = m_text;
	
	size_t position = s.find(needle);

	if (position != std::string::npos)
	{
		return true;
	}

	return false;
}

void UwpEngine::CString::Append(const char* string)
{
	int newLength = m_length + (int)strlen(string);

	char* hold = new char[newLength];

	memset(hold, 0x00, newLength);
	memcpy(hold, m_text, m_length);
	memcpy(&hold[m_length], string, strlen(string));

	delete[] m_text;

	m_length = newLength;
	m_text = new char[m_length + 1];
	memset(m_text, 0x00, m_length + 1);
	memcpy(m_text, hold, m_length);
}

wchar_t* UwpEngine::CString::MBSToWide()
{
	size_t	gfsize;

	mbstowcs_s(&gfsize, m_gfwcs, 1024, m_text, m_length);

	return m_gfwcs;
}