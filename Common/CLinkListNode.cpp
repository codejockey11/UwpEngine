#include "pch.h"

#include "CLinkListNode.h"

UwpEngine::CLinkListNode::CLinkListNode()
{
	memset(this, 0x00, sizeof(CLinkListNode));
}

UwpEngine::CLinkListNode::~CLinkListNode()
{
	delete m_stringKey;
}