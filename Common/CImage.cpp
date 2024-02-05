#include "pch.h"

#include "Common/CImage.h"

UwpEngine::CImage::CImage()
{
	memset(this, 0x00, sizeof(CImage));
}

UwpEngine::CImage::CImage(CVideoDevice* videoDevice, const char* imagename)
{
	memset(this, 0x00, sizeof(CImage));

	m_videoDevice = videoDevice;
	m_imageName = new CString(imagename);

	if (m_imageName->Search(".tga"))
	{
		m_tgaImage = new CTgaImage(m_imageName->m_text);

		if (m_tgaImage->m_isInitialized)
		{
			m_width = m_tgaImage->m_header.width;
			m_height = m_tgaImage->m_header.height;

			m_pitch = m_tgaImage->m_header.width * (m_tgaImage->m_header.pixelDepth / 8);

			switch (m_tgaImage->m_header.pixelDepth)
			{
				case 8:
				{
					m_data = (void*)m_tgaImage->m_pixels8;

					break;
				}
				case 24:
				{
					m_data = (void*)m_tgaImage->m_pixels24;

					break;
				}
				case 32:
				{
					m_data = (void*)m_tgaImage->m_pixels32;

					break;
				}
			}
		}
	}
	else if (m_imageName->Search(".bmp"))
	{
		m_bmpImage = new CBmpImage(m_imageName->m_text);

		if (m_bmpImage->m_isInitialized)
		{
			m_width = m_bmpImage->m_info.bmiHeader.biWidth;
			m_height = m_bmpImage->m_info.bmiHeader.biHeight;

			m_pitch = m_bmpImage->m_info.bmiHeader.biWidth * (m_bmpImage->m_info.bmiHeader.biBitCount / 8);

			m_data = m_bmpImage->m_pixels;
		}
	}
	else
	{
		throw Platform::FailureException::CreateException(E_FAIL);

		return;
	}

	CreateDeviceDependentResources();

	m_isInitialized = true;
}

UwpEngine::CImage::~CImage()
{
	delete m_imageName;

	if (m_tgaImage)
	{
		delete m_tgaImage;
	}

	if (m_bmpImage)
	{
		delete m_bmpImage;
	}
}

void UwpEngine::CImage::CreateDeviceDependentResources()
{
	D2D1_SIZE_U             size;
	D2D1_BITMAP_PROPERTIES1	bitmapProperties;

	size.width = m_width;
	size.height = m_height;

	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_NONE;
	bitmapProperties.colorContext = nullptr;
	bitmapProperties.dpiX = 96;
	bitmapProperties.dpiY = 96;
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

	ThrowIfFailed(
		m_videoDevice->GetD2DDeviceContext()->CreateBitmap(size, m_data, m_pitch, &bitmapProperties, &m_bitmap)
	);
}

void UwpEngine::CImage::ReleaseDeviceDependentResources()
{
	m_bitmap.Reset();
}