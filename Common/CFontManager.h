#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CFont.h"
#include "Common/CLinkList.h"

namespace UwpEngine
{
	class CFontManager
	{
	public:

		CFontManager();
		CFontManager(CVideoDevice* videoDevice);
		~CFontManager();

		CFont* MakeFont(const char* fontname, float size);
		CFont* GetFont(const char* fontname);

	private:

		CVideoDevice*	m_videoDevice;
		CLinkList*		m_fonts;
	};
}