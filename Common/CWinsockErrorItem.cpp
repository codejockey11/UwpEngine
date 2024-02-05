#include "pch.h"

#include "Common/CWinsockErrorItem.h"

UwpEngine::CWinsockErrorItem::CWinsockErrorItem()
{
	memset(this, 0x00, sizeof(CWinsockErrorItem));
}

UwpEngine::CWinsockErrorItem::CWinsockErrorItem(UINT number, const char* name, const char* message)
{
	memset(this, 0x00, sizeof(CWinsockErrorItem));

	m_nbr = number;
	m_name = new CString(name);
	m_message = new CString(message);
}

UwpEngine::CWinsockErrorItem::~CWinsockErrorItem()
{
	delete m_name;
	delete m_message;
}