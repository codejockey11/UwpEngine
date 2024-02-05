#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CString.h"
#include "Common/CTgaImage.h"
#include "Common/CBmpImage.h"

namespace UwpEngine
{
	class CTexture
	{
	public:

		CString*							m_texturename;
		bool								m_isInitialized;
		ComPtr<ID3D11Texture2D>				m_texture;
		ComPtr<ID3D11ShaderResourceView>	m_shaderResourceView;

		CTexture();
		CTexture(CVideoDevice* videoDevice, const char* texturename);
		~CTexture();

		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

	private:

		CVideoDevice*			m_videoDevice;
		D3D11_TEXTURE2D_DESC	m_desc;
		UINT					m_pitch;
		UINT					m_slice;
		void*					m_data;
		CTgaImage*				m_tgaImage;
		CBmpImage*				m_bmpImage;
	};
}