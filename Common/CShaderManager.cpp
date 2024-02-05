#include "pch.h"

#include "Common/CShaderManager.h"

UwpEngine::CShaderManager::CShaderManager()
{
	memset(this, 0x00, sizeof(CShaderManager));
}

UwpEngine::CShaderManager::CShaderManager(CVideoDevice* videoDevice)
{
	memset(this, 0x00, sizeof(CShaderManager));

	m_videoDevice = videoDevice;

	m_shaders = new CLinkList();
}

UwpEngine::CShaderManager::~CShaderManager()
{
	CLinkListNode* shaders = m_shaders->m_list;

	while (shaders)
	{
		CShader* shader = (CShader*)shaders->m_element;

		delete shader;

		shaders = shaders->m_next;
	}

	delete m_shaders;
}

UwpEngine::CShader* UwpEngine::CShaderManager::MakeShader(const char* vertexShaderName, const char* pixelShaderName, D3D11_INPUT_ELEMENT_DESC* elementDesc, UINT elementCount)
{
	CShader* shader = GetShader(vertexShaderName);
	
	if (shader)
	{
		return shader;
	}

	shader = new CShader(m_videoDevice, vertexShaderName, pixelShaderName, elementDesc, elementCount);

	if (shader->m_isInitialized)
	{
		m_shaders->Add(shader, vertexShaderName);

		return shader;
	}

	throw Platform::FailureException::CreateException(E_FAIL);

	delete shader;

	return nullptr;
}

void UwpEngine::CShaderManager::DeleteShader(const char* shaderName)
{
	CLinkListNode* shader = m_shaders->Search(shaderName);

	if (shader)
	{
		m_shaders->Delete(shader);
	}
}

UwpEngine::CShader* UwpEngine::CShaderManager::GetShader(const char* shaderName)
{
	CLinkListNode* shader = m_shaders->Search(shaderName);

	if (shader)
	{
		return (CShader*)shader->m_element;
	}

	return nullptr;
}