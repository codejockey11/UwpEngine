#include "pch.h"

#include "ShapeRenderer.h"

// Initializes D2D resources used for text rendering.
UwpEngine::ShapeRenderer::ShapeRenderer(UwpEngine::CVideoDevice* d)
{
	m_videoDevice = d;

	// Create device independent resources
	ThrowIfFailed(
		m_videoDevice->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);

	CreateDeviceDependentResources();
}

UwpEngine::ShapeRenderer::~ShapeRenderer()
{
	m_pYellowGreenBrush->Release();
	m_pYellowBrush->Release();
}

// Updates the text to be displayed.
void UwpEngine::ShapeRenderer::Update(UwpEngine::CFrameTimer const& timer)
{
}

// Renders a frame to the screen.
void UwpEngine::ShapeRenderer::Render()
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

	context->FillRectangle(&rcBrushRect, m_pYellowGreenBrush);
	context->DrawRectangle(&rcBrushRect, m_pYellowBrush);

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void UwpEngine::ShapeRenderer::CreateDeviceDependentResources()
{
	ThrowIfFailed(
		m_videoDevice->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)), &m_pYellowGreenBrush)
		);

	ThrowIfFailed(
		m_videoDevice->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(0xFFFF00, 1.0f)), &m_pYellowBrush)
	);
}
void UwpEngine::ShapeRenderer::ReleaseDeviceDependentResources()
{
	m_pYellowGreenBrush->Release();
	m_pYellowBrush->Release();
}