#pragma once

#include "Common/CVideoDevice.h"
#include "Common/CFrameTimer.h"

namespace UwpEngine
{
	class CShape
	{
	public:

		CShape();
		CShape(CVideoDevice* videoDevice);
		~CShape();

		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(CFrameTimer const& timer);
		void Render();

	private:
		
		CVideoDevice*					m_videoDevice;
		ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;
		ComPtr<ID2D1SolidColorBrush>	m_pYellowGreenBrush;
		ComPtr<ID2D1SolidColorBrush>	m_pYellowBrush;
	};
}