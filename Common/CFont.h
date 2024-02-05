#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CString.h"

namespace UwpEngine
{
	class CFont
	{
	public:

		CString*	m_fontName;
		float		m_fontSize;
		bool		m_isInitialized;

		CFont();
		CFont(CVideoDevice* videoDevice, const char* fontname, float size);
		~CFont();

		void Update(const wchar_t* text, float width, float height);
		void Render(float x, float y, D2D1::ColorF color);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

	private:
		
		CVideoDevice*					m_videoDevice;
		DWRITE_TEXT_METRICS	            m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_brush;
		ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		ComPtr<IDWriteTextLayout3>      m_textLayout;
		ComPtr<IDWriteTextFormat2>      m_textFormat;
	};
}