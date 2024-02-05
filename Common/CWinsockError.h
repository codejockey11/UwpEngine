#pragma once

#include "pch.h"

#include "Common/CLinkList.h"
#include "Common/CWinsockErrorItem.h"

namespace UwpEngine
{
	class CWinsockError
	{
	public:

		CLinkList*		m_errors;

		CWinsockError();
		~CWinsockError();

		CWinsockErrorItem* GetError(UINT number);
		void AddItem(CWinsockErrorItem* errorItem);
	};
}