#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"
#include "Common/CLinkList.h"
#include "Common/CTexture.h"

namespace UwpEngine
{
	class CTextureManager
	{
	public:

		CTextureManager();
		CTextureManager(CVideoDevice* videoDevice);
		~CTextureManager();

		void DeleteTexture(const char* texturename);
		CTexture* MakeTexture(const char* texturename);
		CTexture* GetTexture(const char* texturename);

	private:

		CVideoDevice*	m_videoDevice;
		CLinkList*		m_textures;
	};
}