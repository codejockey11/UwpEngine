#include "pch.h"

#include "Common/CTexture.h"

UwpEngine::CTexture::CTexture()
{
	memset(this, 0x00, sizeof(CTexture));
}

UwpEngine::CTexture::CTexture(CVideoDevice* videoDevice, const char* texturename)
{
	memset(this, 0x00, sizeof(CTexture));

	m_videoDevice = videoDevice;
	m_texturename = new CString(texturename);
	m_isInitialized = false;

	if (m_texturename->Search(".tga"))
	{
		m_tgaImage = new CTgaImage(m_texturename->m_text);

		if (m_tgaImage->m_isInitialized)
		{
			m_desc.Width = m_tgaImage->m_header.width;
			m_desc.Height = m_tgaImage->m_header.height;
			m_desc.MipLevels = m_desc.ArraySize = 1;
			m_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_desc.SampleDesc.Count = 1;
			m_desc.Usage = D3D11_USAGE_DYNAMIC;
			m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			m_desc.MiscFlags = 0;

			m_pitch = m_desc.Width * (m_tgaImage->m_header.pixelDepth / 8);
			m_slice = m_pitch * m_desc.Height;

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
	else if (m_texturename->Search(".bmp"))
	{
		m_bmpImage = new CBmpImage(m_texturename->m_text);

		if (m_bmpImage->m_isInitialized)
		{
			m_desc.Width = m_bmpImage->m_info.bmiHeader.biWidth;
			m_desc.Height = m_bmpImage->m_info.bmiHeader.biHeight;
			m_desc.MipLevels = m_desc.ArraySize = 1;
			m_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_desc.SampleDesc.Count = 1;
			m_desc.Usage = D3D11_USAGE_DYNAMIC;
			m_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			m_desc.MiscFlags = 0;

			m_pitch = m_desc.Width * (m_bmpImage->m_info.bmiHeader.biBitCount / 8);
			m_slice = m_pitch * m_desc.Height;

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

UwpEngine::CTexture::~CTexture()
{
	delete m_texturename;

	if (m_tgaImage)
	{
		delete m_tgaImage;
	}

	if (m_bmpImage)
	{
		delete m_bmpImage;
	}
}

void UwpEngine::CTexture::CreateDeviceDependentResources()
{
	D3D11_SUBRESOURCE_DATA initData;

	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

	initData.pSysMem = m_data;
	initData.SysMemPitch = m_pitch;
	initData.SysMemSlicePitch = m_slice;

	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateTexture2D(
			&m_desc,
			&initData,
			&m_texture)
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;

	ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	desc.Format = m_desc.Format;
	desc.Texture2D.MipLevels = 1;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Buffer.NumElements = 1;
	
	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateShaderResourceView(
			m_texture.Get(),
			&desc,
			&m_shaderResourceView)
	);
}

void UwpEngine::CTexture::ReleaseDeviceDependentResources()
{
	m_texture.Reset();
	m_shaderResourceView.Reset();
}