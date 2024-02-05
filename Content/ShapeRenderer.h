#pragma once

#include "Common/CVideoDevice.h"
#include "Common/CFrameTimer.h"

namespace UwpEngine
{
	// Renders the current FPS value in the bottom right corner of the screen using Direct2D and DirectWrite.
	class ShapeRenderer
	{
	public:

		ShapeRenderer(CVideoDevice* d);
		~ShapeRenderer();

		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(CFrameTimer const& timer);
		void Render();

	private:
		// Cached pointer to device resources.
		CVideoDevice*					m_videoDevice;

		// Resources related to shape rendering.
		ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		ComPtr<IDWriteTextLayout3>      m_textLayout;
		ComPtr<IDWriteTextFormat2>      m_textFormat;
		
		// Shapes don't use ComPtr's
		ID2D1SolidColorBrush*			m_pYellowGreenBrush;
		ID2D1SolidColorBrush*			m_pYellowBrush;
	};
}