#include "pch.h"

#include "CHudImage.h"

UwpEngine::CHudImage::CHudImage()
{
	memset(this, 0x00, sizeof(CHudImage));
}

UwpEngine::CHudImage::CHudImage(CVideoDevice* videoDevice, CShaderManager* shaderManager, CTextureManager* textureManager, const char* shadername, const char* texturename)
{
	memset(this, 0x00, sizeof(CHudImage));

	m_videoDevice = videoDevice;
	m_isInitialized = false;

	float xScale = (m_videoDevice->GetScreenViewport().Width > 0) ? 2.0f / m_videoDevice->GetScreenViewport().Width : 0.0f;
	float yScale = (m_videoDevice->GetScreenViewport().Height > 0) ? 2.0f / m_videoDevice->GetScreenViewport().Height : 0.0f;

	m_wvpMat = XMMATRIX
	(
		xScale, 0, 0, 0,
		0, -yScale, 0, 0,
		0, 0, 1, 0,
		-1, 1, 0, 1
	);

	m_shader = shaderManager->GetShader(shadername);

	m_texture = textureManager->GetTexture(texturename);

	CreateDeviceDependentResources();

	m_isInitialized = true;
}

UwpEngine::CHudImage::~CHudImage()
{
}

void UwpEngine::CHudImage::Update(CFrameTimer const& timer)
{
	m_vertices[0].color.x += (float)timer.GetElapsedSeconds(); if (m_vertices[0].color.x > 1.0f) { m_vertices[0].color.x = 0.0f; }
	m_vertices[1].color.x += (float)timer.GetElapsedSeconds(); if (m_vertices[1].color.x > 1.0f) { m_vertices[1].color.x = 0.0f; }
	m_vertices[2].color.x += (float)timer.GetElapsedSeconds(); if (m_vertices[2].color.x > 1.0f) { m_vertices[2].color.x = 0.0f; }
	m_vertices[3].color.x += (float)timer.GetElapsedSeconds(); if (m_vertices[3].color.x > 1.0f) { m_vertices[3].color.x = 0.0f; }

	m_vertices[0].color.y += (float)timer.GetElapsedSeconds() * 0.50f; if (m_vertices[0].color.y > 1.0f) { m_vertices[0].color.y = 0.0f; }
	m_vertices[1].color.y += (float)timer.GetElapsedSeconds() * 0.50f; if (m_vertices[1].color.y > 1.0f) { m_vertices[1].color.y = 0.0f; }
	m_vertices[2].color.y += (float)timer.GetElapsedSeconds() * 0.50f; if (m_vertices[2].color.y > 1.0f) { m_vertices[2].color.y = 0.0f; }
	m_vertices[3].color.y += (float)timer.GetElapsedSeconds() * 0.50f; if (m_vertices[3].color.y > 1.0f) { m_vertices[3].color.y = 0.0f; }

	m_vertices[0].color.z += (float)timer.GetElapsedSeconds() * 1.50f; if (m_vertices[0].color.z > 1.0f) { m_vertices[0].color.z = 0.0f; }
	m_vertices[1].color.z += (float)timer.GetElapsedSeconds() * 1.50f; if (m_vertices[1].color.z > 1.0f) { m_vertices[1].color.z = 0.0f; }
	m_vertices[2].color.z += (float)timer.GetElapsedSeconds() * 1.50f; if (m_vertices[2].color.z > 1.0f) { m_vertices[2].color.z = 0.0f; }
	m_vertices[3].color.z += (float)timer.GetElapsedSeconds() * 1.50f; if (m_vertices[3].color.z > 1.0f) { m_vertices[3].color.z = 0.0f; }

	m_vertices[0].position.x += (float)timer.GetElapsedSeconds() * 150.0f; if (m_vertices[0].position.x > 200.0f + 500.0f + 500.0f) { m_vertices[0].position.x = 200.0f + 500.0f; }
	m_vertices[3].position.x += (float)timer.GetElapsedSeconds() * 150.0f; if (m_vertices[3].position.x > 200.0f + 500.0f + 500.0f) { m_vertices[3].position.x = 200.0f + 500.0f; }

	m_vertices[1].position.x += (float)timer.GetElapsedSeconds() * 150.0f; if (m_vertices[1].position.x > 0.0f + 500.0f + 500.0f) { m_vertices[1].position.x = 0.0f + 500.0f; }
	m_vertices[2].position.x += (float)timer.GetElapsedSeconds() * 150.0f; if (m_vertices[2].position.x > 0.0f + 500.0f + 500.0f) { m_vertices[2].position.x = 0.0f + 500.0f; }

	D3D11_MAPPED_SUBRESOURCE resource;

	auto context = m_videoDevice->GetD3DDeviceContext();

	context->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &resource);
	memcpy(resource.pData, m_vertices, sizeof(VertexPositionTextureColor) * 4);
	context->Unmap(m_vertexBuffer.Get(), 0);
}

void UwpEngine::CHudImage::Draw()
{
	UINT stride = sizeof(VertexPositionTextureColor);
	UINT offset = 0;

	auto context = m_videoDevice->GetD3DDeviceContext();

	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_shader->SetWorldViewProjection(m_wvpMat);
	m_shader->SetTextureResource(0, m_texture);
	m_shader->Render();

	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}

void UwpEngine::CHudImage::CreateDeviceDependentResources()
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// 2---3
	// |   |
	// 1---0

	// Load the vertex array with data.
	m_vertices[0].position = XMFLOAT3(200.0f + 500.0f, 200.0f + 600.0f, 0.0f);
	m_vertices[0].texture = XMFLOAT2(1.0f, 1.0f);
	m_vertices[0].color = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_vertices[1].position = XMFLOAT3(0.0f + 500.0f, 200.0f + 600.0f, 0.0f);
	m_vertices[1].texture = XMFLOAT2(0.0f, 1.0f);
	m_vertices[1].color = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_vertices[2].position = XMFLOAT3(0.0f + 500.0f, 0.0f + 400.0f, 0.0f);
	m_vertices[2].texture = XMFLOAT2(0.0f, 0.0f);
	m_vertices[2].color = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_vertices[3].position = XMFLOAT3(200.0f + 500.0f, 0.0f + 400.0f, 0.0f);
	m_vertices[3].texture = XMFLOAT2(1.0f, 0.0f);
	m_vertices[3].color = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 2;
	m_indices[3] = 0;
	m_indices[4] = 2;
	m_indices[5] = 3;

	m_indexCount = 6;

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionTextureColor) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)
	);

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = m_indices;

	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)
	);
}

void UwpEngine::CHudImage::ReleaseDeviceDependentResources()
{
	m_isInitialized = false;
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}