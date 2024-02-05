#pragma once

#include "pch.h"

#include "Common/CString.h"

namespace UwpEngine
{
	class CWinsockErrorItem
	{
	public:

		UINT			m_nbr;
		CString*		m_name;
		CString*		m_message;

		CWinsockErrorItem();
		CWinsockErrorItem(UINT number, const char* name, const char* message);
		~CWinsockErrorItem();
	};
}