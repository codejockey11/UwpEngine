#pragma once

#include "Common/CFrameTimer.h"
#include "Common/CCamera.h"
#include "Common/CServer.h"
#include "Common/CClient.h"

#include "Common/CShape.h"
#include "Common/CSoundManager.h"
#include "Common/CFontManager.h"
#include "Common/CTextureManager.h"
#include "Common/CImageManager.h"
#include "Common/CShaderManager.h"
#include "Common/CModel.h"
//#include "Common/CModelManager.h"
#include "Common/CHudImage.h"
//#include "Common/CHudImageManager.h"

namespace UwpEngine
{
	class EventManager : public UwpEngine::IDeviceNotify
	{
	public:
		
		CServer*	m_server;
		CClient*	m_client;

		bool		m_isRunning;
		bool		m_consoleVisible;
		bool		m_isExiting;

		EventManager();
		EventManager(CVideoDevice* videoDevice, CSoundManager* soundManager, CFontManager* fontManager, CTextureManager* textureManager, CImageManager* imageManager, CShaderManager* shaderManager);
		~EventManager();
		
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();
		void LoadSounds();
		void LoadFonts();
		void LoadShaders();
		void LoadTextures();
		void LoadImages();
		void LoadHudImages();
		void LoadModels();
		void SendEvent(CPacket* p);
		void ProcessEvent(CPacket* p);

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		
		CVideoDevice*		m_videoDevice;
		CSoundDevice*		m_soundDevice;

		CShape*				m_shape;
		CSoundManager*		m_soundManager;
		CFontManager*		m_fontManager;
		CTextureManager*	m_textureManager;
		CImageManager*		m_imageManager;
		CShaderManager*		m_shaderManager;
		CCamera*			m_camera;
		CModel*				m_model;
		CHudImage*			m_hudImage;

		CFrameTimer			m_timer;

		HANDLE				m_hThread;
		UINT				m_recvThreadId;
		std::string			m_chatText;
		size_t				m_pingRate;

		void StartServer();
		void ClientConnect(CPacket* p);
		void Exit(CPacket* p);
	};
}