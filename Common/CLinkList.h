#pragma once

#include "pch.h"

#include "CLinkListNode.h"

namespace UwpEngine
{
	class CLinkList
	{
	public:

		CLinkListNode*	m_list;
		int				m_count;

		CLinkList();
		~CLinkList();

		void Clear();
		void Add(void* element, const char* stringKey);
		void Add(void* element, int numberKey);

		CLinkListNode* Delete(CLinkListNode* node);
		CLinkListNode* Search(const char* stringKey);
		CLinkListNode* Search(int numberKey);
	};
}