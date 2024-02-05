#include "pch.h"

#include "CShader.h"

UwpEngine::CShader::CShader()
{
	memset(this, 0x00, sizeof(CShader));
}

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
UwpEngine::CShader::CShader(CVideoDevice* videoDevice, const char* vertexShaderName, const char* pixelShaderName, D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount)
{
	memset(this, 0x00, sizeof(CShader));

	m_videoDevice = videoDevice;
	m_vertexName = new CString(vertexShaderName);
	m_pixelName = new CString(pixelShaderName);
	m_vertexDesc = elementDesc;
	m_elementCount = elementCount;
	m_isInitialized = false;
		
	CreateDeviceDependentResources();
}

UwpEngine::CShader::~CShader()
{
	delete m_vertexName;
	delete m_pixelName;
}

void UwpEngine::CShader::SetWorldViewProjection(XMMATRIX wvp)
{
	XMStoreFloat4x4(&m_constantBufferData.wvp, wvp);
}

void UwpEngine::CShader::SetTextureResource(UINT startSlot, CTexture* texture)
{
	auto context = m_videoDevice->GetD3DDeviceContext();

	context->PSSetShaderResources(
		startSlot,
		1,
		texture->m_shaderResourceView.GetAddressOf()
	);
}

void UwpEngine::CShader::Render()
{
	if (!m_isInitialized)
	{
		return;
	}

	auto context = m_videoDevice->GetD3DDeviceContext();

	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
	);

	context->IASetInputLayout(m_inputLayout.Get());

	context->PSSetSamplers(
		0,
		1,
		m_samplerAnisotropic.GetAddressOf()
	);

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
	);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
	);
}

void UwpEngine::CShader::CreateDeviceDependentResources()
{
	int err = fopen_s(&m_file, m_vertexName->m_text, "rb");

	long size;
	fseek(m_file, 0L, SEEK_END);
	size = ftell(m_file);
	fseek(m_file, 0L, SEEK_SET);

	char* bytes = new char[size];
	fread(bytes, size, 1, m_file);

	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateVertexShader(
			bytes,
			size,
			nullptr,
			&m_vertexShader
		)
	);

	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateInputLayout(
			m_vertexDesc,
			m_elementCount,
			bytes,
			size,
			&m_inputLayout
		)
	);

	delete[] bytes;

	fclose(m_file);


	fopen_s(&m_file, m_pixelName->m_text, "rb");

	fseek(m_file, 0L, SEEK_END);
	size = ftell(m_file);
	fseek(m_file, 0L, SEEK_SET);

	bytes = new char[size];
	fread(bytes, size, 1, m_file);

	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreatePixelShader(
			bytes,
			size,
			nullptr,
			&m_pixelShader
		)
	);

	delete[] bytes;

	fclose(m_file);

	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(WorldViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&m_constantBuffer
		)
	);

	D3D11_SAMPLER_DESC samplerDesc;

	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateSamplerState(
			&samplerDesc,
			&m_samplerAnisotropic)
	);

	m_isInitialized = true;
}

void UwpEngine::CShader::ReleaseDeviceDependentResources()
{
	m_isInitialized = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_samplerAnisotropic.Reset();
}