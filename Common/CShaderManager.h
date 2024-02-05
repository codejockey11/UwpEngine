#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CCamera.h"
#include "Common/CShader.h"
#include "Common/CLinkList.h"

namespace UwpEngine
{
	class CShaderManager
	{
	public:

		CShaderManager();
		CShaderManager(CVideoDevice* videoDevice);
		~CShaderManager();

		CShader* MakeShader(const char* vertexShaderName, const char* pixelShaderName, D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount);
		CShader* GetShader(const char* shaderName);
		void DeleteShader(const char* shaderName);

	private:

		CVideoDevice*	m_videoDevice;
		CLinkList*		m_shaders;
	};
}