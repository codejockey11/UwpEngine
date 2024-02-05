#pragma once

#include "pch.h"

#include "Common/CString.h"

namespace UwpEngine
{
	class CLinkListNode
	{
	public:

		CLinkListNode*		m_previous;
		CLinkListNode*		m_next;
		CString*			m_stringKey;
		int					m_intKey;
		bool				m_deleteMe;
		void*				m_element;

		CLinkListNode();
		~CLinkListNode();
	};
}