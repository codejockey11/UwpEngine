#include "pch.h"

#include "Common/CTextureManager.h"

UwpEngine::CTextureManager::CTextureManager()
{
	memset(this, 0x00, sizeof(CTextureManager));
}

UwpEngine::CTextureManager::CTextureManager(CVideoDevice* videoDevice)
{
	memset(this, 0x00, sizeof(CTextureManager));

	m_videoDevice = videoDevice;

	m_textures = new CLinkList();
}

UwpEngine::CTextureManager::~CTextureManager()
{
	CLinkListNode* textures = m_textures->m_list;

	while (textures)
	{
		CTexture* texture = (CTexture*)textures->m_element;

		delete texture;

		textures = textures->m_next;
	}

	delete m_textures;
}

UwpEngine::CTexture* UwpEngine::CTextureManager::MakeTexture(const char* texturename)
{
	CTexture* texture = GetTexture(texturename);
	
	if (texture)
	{
		return texture;
	}

	texture = new CTexture(m_videoDevice, texturename);

	if (texture->m_isInitialized)
	{
		m_textures->Add(texture, texturename);

		return texture;
	}

	throw Platform::FailureException::CreateException(E_FAIL);

	delete texture;

	return nullptr;
}

void UwpEngine::CTextureManager::DeleteTexture(const char* texturename)
{
	CLinkListNode* texture =  m_textures->Search(texturename);

	if (texture)
	{
		m_textures->Delete(texture);
	}
}

UwpEngine::CTexture* UwpEngine::CTextureManager::GetTexture(const char* texturename)
{
	CLinkListNode* texture = m_textures->Search(texturename);

	if (texture)
	{
		return (UwpEngine::CTexture*)texture->m_element;
	}

	return nullptr;
}