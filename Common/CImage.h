#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CString.h"
#include "Common/CTgaImage.h"
#include "Common/CBmpImage.h"

namespace UwpEngine
{
	class CImage
	{
	public:

		CString*				m_imageName;
		bool					m_isInitialized;
		ComPtr<ID2D1Bitmap1>	m_bitmap;

		CImage();
		CImage(CVideoDevice* videoDevice, const char* imagename);
		~CImage();

		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

	private:

		UINT				m_width;
		UINT				m_height;
		UINT				m_pitch;
		void*				m_data;
		CTgaImage*			m_tgaImage;
		CBmpImage*			m_bmpImage;
		CVideoDevice*		m_videoDevice;
	};
}