#pragma once

#include "pch.h"

#include "Common/CImage.h"
#include "Common/CLinkList.h"
#include "Common/CVideoDevice.h"

namespace UwpEngine
{
	class CImageManager
	{
	public:

		CImageManager();
		CImageManager(CVideoDevice* videoDevice);
		~CImageManager();

		void DeleteImage(const char* imagename);
		CImage* MakeImage(const char* imagename);
		CImage* GetImage(const char* imagename);

	private:

		CVideoDevice*	m_videoDevice;
		CLinkList*		m_images;
	};
}