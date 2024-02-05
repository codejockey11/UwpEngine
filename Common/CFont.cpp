#include "pch.h"

#include "CFont.h"

UwpEngine::CFont::CFont()
{
	memset(this, 0x00, sizeof(CFont));
}

UwpEngine::CFont::CFont(CVideoDevice* videoDevice, const char* fontname, float size)
{
	memset(this, 0x00, sizeof(CFont));

	m_videoDevice = videoDevice;
	m_fontName = new CString(fontname);
	m_fontSize = size;
	m_isInitialized = false;
	
	// Create device independent resources
	ComPtr<IDWriteTextFormat> textFormat;
	ThrowIfFailed(
		m_videoDevice->GetDWriteFactory()->CreateTextFormat(
			m_fontName->MBSToWide(),
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_fontSize,
			L"en-US",
			&textFormat
		)
	);

	ThrowIfFailed(
		textFormat.As(&m_textFormat)
	);

	ThrowIfFailed(
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
	);

	ThrowIfFailed(
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
	);

	ThrowIfFailed(
		m_videoDevice->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
	);

	CreateDeviceDependentResources();

	m_isInitialized = true;
}

UwpEngine::CFont::~CFont()
{
	delete m_fontName;
}

void UwpEngine::CFont::Update(const wchar_t* text, float width, float height)
{
	ComPtr<IDWriteTextLayout> textLayout;
	ThrowIfFailed(
		m_videoDevice->GetDWriteFactory()->CreateTextLayout(
			text,
			(uint32)wcslen(text),
			m_textFormat.Get(),
			width, // Max width of the input text.
			height, // Max height of the input text.
			&textLayout
		)
	);

	ThrowIfFailed(
		textLayout.As(&m_textLayout)
	);

	ThrowIfFailed(
		textLayout->GetMetrics(&m_textMetrics)
	);
}

void UwpEngine::CFont::Render(float x, float y, D2D1::ColorF color)
{
	m_brush->SetColor(color);

	ID2D1DeviceContext* context = m_videoDevice->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_videoDevice->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	context->DrawTextLayout(
		D2D1::Point2F(x, y),
		m_textLayout.Get(),
		m_brush.Get()
	);

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void UwpEngine::CFont::CreateDeviceDependentResources()
{
	ThrowIfFailed(
		m_videoDevice->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_brush)
	);
}

void UwpEngine::CFont::ReleaseDeviceDependentResources()
{
	m_brush.Reset();
}