#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CFrameTimer.h"
#include "Common/CShaderManager.h"
#include "Common/CTextureManager.h"

namespace UwpEngine
{
	struct VertexPositionTextureColor
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 color;
	};

	class CHudImage
	{
	public:

		XMMATRIX		m_wvpMat;
		bool			m_isInitialized;

		CHudImage();
		CHudImage(CVideoDevice* videoDevice, CShaderManager* shaderManager, CTextureManager* textureManager, const char* shadername, const char* texturename);
		~CHudImage();

		void Update(CFrameTimer const& timer);
		void Draw();
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

	private:

		CVideoDevice*				m_videoDevice;
		ComPtr<ID3D11Buffer>		m_vertexBuffer;
		ComPtr<ID3D11Buffer>		m_indexBuffer;
		UINT						m_indexCount;
		CTexture*					m_texture;
		CShader*					m_shader;
		VertexPositionTextureColor	m_vertices[4];
		UINT						m_indices[6];
	};
}