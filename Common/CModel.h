#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CString.h"
#include "Common/CCamera.h"
#include "Common/CFrameTimer.h"
#include "Common/CShaderManager.h"

namespace UwpEngine
{
	struct VertexPositionColor
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

	static const VertexPositionColor cubeVertices[] =
	{
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
		{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
	};

	static const unsigned short cubeIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	class CModel
	{
	public:

		CString*		m_modelName;
		bool			m_isInitialized;
		double			m_degreesPerSecond;

		CModel();
		CModel(CVideoDevice* videoDevice, CShaderManager* shaderManager, CCamera* camera, const char* modelname);
		~CModel();

		void Update();
		void Render();
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

	private:
		
		CVideoDevice*			m_videoDevice;
		CCamera*				m_camera;
		ComPtr<ID3D11Buffer>	m_vertexBuffer;
		ComPtr<ID3D11Buffer>	m_indexBuffer;
		UINT					m_indexCount;
		CShaderManager*			m_shaderManager;
		CShader*				m_shader;
	};
}