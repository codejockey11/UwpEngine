#include "pch.h"

#include "CModel.h"

UwpEngine::CModel::CModel()
{
	memset(this, 0x00, sizeof(CModel));
}

UwpEngine::CModel::CModel(CVideoDevice* videoDevice, CShaderManager* shaderManager, CCamera* camera, const char* modelname)
{
	memset(this, 0x00, sizeof(CModel));

	m_videoDevice = videoDevice;
	m_camera = camera;
	m_modelName = new CString(modelname);
	m_isInitialized = false;
	m_indexCount = 36;

	m_shader = shaderManager->GetShader("SampleVertexShader.cso");

	CreateDeviceDependentResources();

	m_isInitialized = true;
}

UwpEngine::CModel::~CModel()
{
	delete m_modelName;
}

void UwpEngine::CModel::Update()
{
}

void UwpEngine::CModel::Render()
{
	if (!m_isInitialized)
	{
		return;
	}

	auto context = m_videoDevice->GetD3DDeviceContext();

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
	);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_shader->SetWorldViewProjection(m_camera->m_wvpMat);
	m_shader->Render();

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}

void UwpEngine::CModel::CreateDeviceDependentResources()
{
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	
	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
		)
	);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	
	ThrowIfFailed(
		m_videoDevice->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_indexBuffer
		)
	);
}

void UwpEngine::CModel::ReleaseDeviceDependentResources()
{
	m_isInitialized = false;
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}