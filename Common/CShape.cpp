#include "pch.h"

#include "Common/CShape.h"

UwpEngine::CShape::CShape()
{
	memset(this, 0x00, sizeof(CShape));
}

UwpEngine::CShape::CShape(UwpEngine::CVideoDevice* videoDevice)
{
	memset(this, 0x00, sizeof(CShape));

	m_videoDevice = videoDevice;

	ThrowIfFailed(
		m_videoDevice->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
	);

	CreateDeviceDependentResources();
}

UwpEngine::CShape::~CShape()
{
}

// Updates the text to be displayed.
void UwpEngine::CShape::Update(CFrameTimer const& timer)
{
}

// Renders a frame to the screen.
void UwpEngine::CShape::Render()
{
	ID2D1DeviceContext* context = m_videoDevice->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_videoDevice->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	D2D1_RECT_F rcBrushRect;
	
	rcBrushRect.left = 10;
	rcBrushRect.bottom = 200;
	rcBrushRect.right = 200;
	rcBrushRect.top = 10;

	context->FillRectangle(&rcBrushRect, m_pYellowGreenBrush.Get());
	context->DrawRectangle(&rcBrushRect, m_pYellowBrush.Get());

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void UwpEngine::CShape::CreateDeviceDependentResources()
{
	ThrowIfFailed(
		m_videoDevice->GetD2DDeviceContext()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)),
			m_pYellowGreenBrush.GetAddressOf())
	);

	ThrowIfFailed(
		m_videoDevice->GetD2DDeviceContext()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF(0xFFFF00, 1.0f)),
			m_pYellowBrush.GetAddressOf())
	);
}
void UwpEngine::CShape::ReleaseDeviceDependentResources()
{
	m_pYellowGreenBrush.Reset();
	m_pYellowBrush.Reset();
}