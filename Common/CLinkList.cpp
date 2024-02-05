#include "pch.h"

#include "CLinkList.h"

UwpEngine::CLinkList::CLinkList()
{
	memset(this, 0x00, sizeof(CLinkList));

	m_list = new CLinkListNode();
}

UwpEngine::CLinkList::~CLinkList()
{
	CLinkListNode* traverser = m_list;

	while (traverser)
	{
		CLinkListNode* n = traverser->m_next;

		delete traverser;

		traverser = n;
	}

	m_list = nullptr;
}

void UwpEngine::CLinkList::Clear()
{
	CLinkListNode* p = m_list;

	while (p->m_element)
	{
		p = CLinkList::Delete(p);
	}
}

void UwpEngine::CLinkList::Add(void* element, const char* stringKey)
{
	if (m_list->m_element == 0)
	{
		m_list->m_element = element;
		m_list->m_stringKey = new CString(stringKey);
		m_list->m_next = new CLinkListNode();

		m_count++;

		return;
	}

	CLinkListNode* previous = 0;
	CLinkListNode* traverser = m_list;

	while (traverser->m_element)
	{
		previous = traverser;
		traverser = traverser->m_next;
	}

	traverser->m_previous = previous;
	traverser->m_element = element;
	traverser->m_stringKey = new CString(stringKey);
	traverser->m_next = new CLinkListNode();

	m_count++;
}

void UwpEngine::CLinkList::Add(void* element, int intKey)
{
	if (m_list->m_element == 0)
	{
		m_list->m_element = element;
		m_list->m_intKey = intKey;
		m_list->m_next = new CLinkListNode();

		m_count++;

		return;
	}

	CLinkListNode* previous = 0;
	CLinkListNode* traverser = m_list;

	while (traverser->m_element)
	{
		previous = traverser;
		traverser = traverser->m_next;
	}

	traverser->m_previous = previous;
	traverser->m_element = element;
	traverser->m_intKey = intKey;
	traverser->m_next = new CLinkListNode();

	m_count++;
}

UwpEngine::CLinkListNode* UwpEngine::CLinkList::Delete(CLinkListNode* n)
{
	if (m_list == n)
	{
		m_list->m_next->m_previous = 0;

		CLinkListNode* next = m_list->m_next;

		delete m_list;

		m_list = next;

		m_count--;

		if (m_count == 0)
		{
			m_list->m_next = new CLinkListNode();
		}

		return m_list;
	}

	CLinkListNode* traverser = m_list;

	while (traverser->m_element)
	{
		if (traverser == n)
		{
			CLinkListNode* next = traverser->m_next;

			traverser->m_previous->m_next = traverser->m_next;
			traverser->m_next->m_previous = traverser->m_previous;

			delete traverser;

			m_count--;

			return next;
		}

		traverser = traverser->m_next;
	}

	return nullptr;
}

UwpEngine::CLinkListNode* UwpEngine::CLinkList::Search(const char* stringKey)
{
	CLinkListNode* traverser = m_list;

	while (traverser->m_element)
	{
		if (strcmp(traverser->m_stringKey->m_text, stringKey) == 0)
		{
			return traverser;
		}

		traverser = traverser->m_next;
	}

	return nullptr;
}

UwpEngine::CLinkListNode* UwpEngine::CLinkList::Search(int intKey)
{
	CLinkListNode* traverser = m_list;

	while (traverser->m_element)
	{
		if (traverser->m_intKey == intKey)
		{
			return traverser;
		}

		traverser = traverser->m_next;
	}

	return nullptr;
}