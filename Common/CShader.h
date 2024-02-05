#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CString.h"
#include "Common/CCamera.h"
#include "Common/CTexture.h"

namespace UwpEngine
{
	struct WorldViewProjectionConstantBuffer
	{
		XMFLOAT4X4 wvp;
	};

	// Used to send per-vertex data to the vertex shader.
	static D3D11_INPUT_ELEMENT_DESC vertexPositionColor[2] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static D3D11_INPUT_ELEMENT_DESC vertexPositionTextureColor[3] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	class CShader
	{
	public:

		CString*		m_vertexName;
		CString*		m_pixelName;
		bool			m_isInitialized;
		
		WorldViewProjectionConstantBuffer	m_constantBufferData;

		CShader();
		CShader(CVideoDevice* videoDevice, const char* vertexShaderName, const char* pixelShaderName, D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount);
		~CShader();

		void SetWorldViewProjection(XMMATRIX wvp);
		void SetTextureResource(UINT startSlot, CTexture* texture);
		void Render();
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

	private:

		FILE*								m_file;
		CVideoDevice*						m_videoDevice;
		ComPtr<ID3D11InputLayout>			m_inputLayout;
		D3D11_INPUT_ELEMENT_DESC*			m_vertexDesc;
		UINT								m_elementCount;
		ComPtr<ID3D11VertexShader>			m_vertexShader;
		ComPtr<ID3D11PixelShader>			m_pixelShader;
		ComPtr<ID3D11Buffer>				m_constantBuffer;
		ComPtr<ID3D11SamplerState>			m_samplerAnisotropic;
	};
}