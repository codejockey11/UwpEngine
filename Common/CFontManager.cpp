#include "pch.h"

#include "Common/CFontManager.h"

UwpEngine::CFontManager::CFontManager()
{
	memset(this, 0x00, sizeof(CFontManager));
}

UwpEngine::CFontManager::CFontManager(CVideoDevice* videoDevice)
{
	memset(this, 0x00, sizeof(CFontManager));

	m_videoDevice = videoDevice;

	m_fonts = new CLinkList();
}

UwpEngine::CFontManager::~CFontManager()
{
	CLinkListNode* fonts = m_fonts->m_list;

	while (fonts)
	{
		CFont* font = (CFont*)fonts->m_element;

		delete font;

		fonts = fonts->m_next;
	}

	delete m_fonts;
}

UwpEngine::CFont* UwpEngine::CFontManager::MakeFont(const char* fontname, float size)
{
	CFont* font = GetFont(fontname);

	if (font)
	{
		return font;
	}

	font = new CFont(m_videoDevice, fontname, size);

	if (font->m_isInitialized)
	{
		m_fonts->Add(font, fontname);

		return font;
	}

	throw Platform::FailureException::CreateException(E_FAIL);

	return nullptr;
}

UwpEngine::CFont* UwpEngine::CFontManager::GetFont(const char* fontname)
{
	CLinkListNode* font = m_fonts->Search(fontname);

	if (font)
	{
		return (UwpEngine::CFont*)font->m_element;
	}

	return nullptr;
}