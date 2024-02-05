#include "pch.h"

#include "EventManager.h"

unsigned __stdcall ClientReceiveThread(void* c);

UwpEngine::EventManager::EventManager()
{
}

// Loads and initializes application assets when the application is loaded.
UwpEngine::EventManager::EventManager(CVideoDevice* videoDevice, CSoundManager* soundManager, CFontManager* fontManager, CTextureManager* textureManager, CImageManager* imageManager, CShaderManager* shaderManager)
{
	m_videoDevice = videoDevice;
	m_soundManager = soundManager;
	m_fontManager = fontManager;
	m_textureManager = textureManager;
	m_imageManager = imageManager;
	m_shaderManager = shaderManager;

	// Register to be notified if the Device is lost or recreated
	m_videoDevice->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_shape = new CShape(m_videoDevice);

	m_camera = new CCamera(m_videoDevice);

	m_camera->m_pos.x = 0.0F;
	m_camera->m_pos.y = 0.0F;
	m_camera->m_pos.z = -4.0F;

	m_camera->UpdateRotation(0.0F, 0.0F, 0.0F);
	m_camera->UpdateView();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	timer.SetFixedTimeStep(true);
	timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

UwpEngine::EventManager::~EventManager()
{
	// Deregister device notification
	m_videoDevice->RegisterDeviceNotify(nullptr);

	delete m_shape;
	delete m_camera;
	delete m_model;
	delete m_hudImage;
	delete m_client;
	delete m_server;
}

// Updates application state when the window size changes (e.g. device orientation change)
void UwpEngine::EventManager::CreateWindowSizeDependentResources()
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
}

// Updates the application state once per frame.
void UwpEngine::EventManager::Update()
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_shape->Update(m_timer);
		//m_hudImage->Update(m_timer);
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool UwpEngine::EventManager::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_videoDevice->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_videoDevice->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_videoDevice->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_videoDevice->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_videoDevice->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_videoDevice->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.

	m_model->Render();
	m_shape->Render();

	m_videoDevice->GetD2DDeviceContext()->BeginDraw();

	D2D1_RECT_F r;

	r.left = 10;
	r.bottom = 400;
	r.right = 200;
	r.top = 10;

	m_videoDevice->GetD2DDeviceContext()->DrawBitmap(m_imageManager->GetImage("./Assets/starryAlpha.tga")->m_bitmap.Get(), &r);

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	m_videoDevice->GetD2DDeviceContext()->EndDraw();
	

	uint32 fps = m_timer.GetFramesPerSecond();

	std::wstring text = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";

	CFont* font = m_fontManager->GetFont("Consolas");
	if (font)
	{
		font->Update(text.c_str(), 240.0F, 50.0F);
		font->Render(13.0F, 13.0F, D2D1::ColorF(D2D1::ColorF::Yellow));


		text = std::to_wstring(m_timer.GetElapsedSeconds());

		font = m_fontManager->GetFont("Calibri");
		font->Update(text.c_str(), 240.0F, 50.0F);
		font->Render(13.0F, 43.0F, D2D1::ColorF(D2D1::ColorF::Aquamarine));
	}


	m_hudImage->Draw();

	return true;
}

// Notifies renderers that device resources need to be released.
void UwpEngine::EventManager::OnDeviceLost()
{
	m_fontManager->GetFont("Consolas")->ReleaseDeviceDependentResources();
	m_fontManager->GetFont("Calibri")->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void UwpEngine::EventManager::OnDeviceRestored()
{
	m_fontManager->GetFont("Consolas")->CreateDeviceDependentResources();
	m_fontManager->GetFont("Calibri")->CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}

void UwpEngine::EventManager::LoadSounds()
{
	m_soundManager->MakeSound("./Assets/rifle.wav", false);
	m_soundManager->MakeSound("./Assets/shotgun.wav", false);
}

void UwpEngine::EventManager::LoadFonts()
{
	m_fontManager->MakeFont("Consolas", 24.0F);
	m_fontManager->MakeFont("Calibri", 24.0F);
}

void UwpEngine::EventManager::LoadShaders()
{
	m_shaderManager->MakeShader("SampleVertexShader.cso", "SamplePixelShader.cso", vertexPositionColor, 2);
	m_shaderManager->MakeShader("HudVertex.cso", "HudPixel.cso", vertexPositionTextureColor, 3);
}

void UwpEngine::EventManager::LoadTextures()
{
	m_textureManager->MakeTexture("./Assets/24bitu.tga");
	m_textureManager->MakeTexture("./Assets/starryAlpha.tga");
}

void UwpEngine::EventManager::LoadImages()
{
	m_imageManager->MakeImage("./Assets/24bitu.tga");
	m_imageManager->MakeImage("./Assets/starryAlpha.tga");
}

void UwpEngine::EventManager::LoadHudImages()
{
	m_hudImage = new CHudImage(m_videoDevice, m_shaderManager, m_textureManager, "HudVertex.cso", "./Assets/starryAlpha.tga");
}

void UwpEngine::EventManager::LoadModels()
{
	m_model = new CModel(m_videoDevice, m_shaderManager, m_camera, "model.md3");
}

void UwpEngine::EventManager::Exit(CPacket* p)
{
	m_isExiting = true;

	if (m_client)
	{
		if (m_client->m_connectionState == CClient::CS_CONNECTED)
		{
			m_isRunning = false;

			DWORD rv = WaitForSingleObjectEx(m_hThread, 2000, false);

			CloseHandle(m_hThread);

			m_client->Disconnect();
		}
	}

	if (m_server)
	{
		if (m_server->m_acceptRunning)
		{
			m_server->Shutdown();
		}
	}
}

void UwpEngine::EventManager::StartServer()
{
	if (m_server)
	{
		if (m_server->m_acceptRunning)
		{
			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Server Already Running\n");

			return;
		}

		delete m_server;
	}

	//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Starting Server\n");

	m_server = new CServer("26105");
}

void UwpEngine::EventManager::ClientConnect(CPacket* p)
{
	if (m_client)
	{
		if (m_client->m_connectionState == CClient::CS_CONNECTED)
		{
			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Already Connected:%i\n", p->fromClient);

			return;
		}

		delete m_client;
	}

	//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Connect\n");

	m_client = new CClient(p);

	m_isRunning = true;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, &ClientReceiveThread, (void*)this, 0, &m_recvThreadId);
}

void UwpEngine::EventManager::SendEvent(CPacket* p)
{
	if ((m_client == nullptr) || (p->m_audience == CPacket::PT_LOCAL))
	{
		ProcessEvent(p);

		return;
	}

	m_client->SendEvent(p);
}

void UwpEngine::EventManager::ProcessEvent(CPacket* p)
{
	if (p == nullptr)
	{
		return;
	}

	switch (p->m_type)
	{
		case CPacket::PT_STARTSERVER:
		{
			StartServer();

			break;
		}

		case CPacket::PT_STOPSERVER:
		{
			if (!m_server)
			{
				//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Server Not Running\n");

				break;
			}

			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Stopping Server\n");

			m_server->Shutdown();

			break;
		}

		case CPacket::PT_CONNECT:
		{
			ClientConnect(p);

			break;
		}

		case CPacket::PT_DISCONNECT:
		{
			if (m_client)
			{
				if (m_client->m_connectionState != CClient::CS_CONNECTED)
				{
					//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Not Connected\n");

					break;
				}
			}
			else
			{
				break;
			}

			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Disconnect:%i\n", p->fromClient);

			m_isRunning = false;

			DWORD rv = WaitForSingleObjectEx(m_hThread, 2000, false);

			CloseHandle(m_hThread);

			m_client->Disconnect();


			CPacket* packet = new CPacket(p->m_fromClient, p->m_toClient, CPacket::PT_LOCAL, CPacket::PT_PLAYEREXIT, CPacket::PT_EMPTY, 1);

			SendEvent(packet);

			delete packet;

			break;
		}

		case CPacket::PT_ACCEPTED:
		{
			//globalObjects->errorLog->WriteError("CEventManager::ProcessEvent:Accepted:%i\n", p->fromClient);

			m_client->m_clientNbr = p->m_toClient;

			m_client->m_connectionState = CClient::CS_CONNECTED;

			break;
		}

		case CPacket::PT_EXIT:
		{
			CoreApplication::Exit();

			break;
		}

		case CPacket::PT_ATTACK:
		{
			m_soundManager->GetSound("./Assets/shotgun.wav")->StartSound();

			break;
		}

		case CPacket::PT_FPSMOVE:
		{
			float y, x, z, s;

			memcpy(&y, &p->m_data[0], 4);
			memcpy(&x, &p->m_data[4], 4);
			memcpy(&z, &p->m_data[8], 4);
			memcpy(&s, &p->m_data[12], 4);

			m_camera->UpdateRotation((y * s), (x * s), (z * s));
			m_camera->UpdateView();

			break;
		}
	}
}

unsigned __stdcall ClientReceiveThread(void* c)
{
	UwpEngine::EventManager* ec = (UwpEngine::EventManager*)c;

	while (ec->m_isRunning)
	{
		if (ec->m_client)
		{
			if (ec->m_client->m_connectionState == UwpEngine::CClient::CS_NOTCONNECTED)
			{
				ec->m_isRunning = false;
			}
			else
			{
				ec->m_client->ClientReceive();

				if (ec->m_client->m_connectionState == UwpEngine::CClient::CS_NOTCONNECTED)
				{
					ec->m_isRunning = false;
				}
				else if (ec->m_client->m_packet->m_type)
				{
					ec->ProcessEvent(ec->m_client->m_packet);
				}
			}
		}
	}

	_endthreadex(0);

	return 0;
}