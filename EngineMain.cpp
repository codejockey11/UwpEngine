#include "pch.h"

#include "EngineMain.h"

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);

	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new UwpEngine::EngineMain();
}

UwpEngine::EngineMain::EngineMain()
{
	m_windowClosed = false;
	m_windowVisible = true;
}

// The first method called when the IFrameworkView is being created.
void UwpEngine::EngineMain::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &EngineMain::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &EngineMain::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &EngineMain::OnResuming);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_videoDevice = new CVideoDevice();
	m_soundDevice = new CSoundDevice();
	m_soundManager = new CSoundManager(m_soundDevice);
	m_mouseDevice = new CMouseDevice(m_videoDevice->GetOutputSize(), 0.125F);
	m_keyboardDevice = new CKeyboardDevice();
	m_fontManager = new CFontManager(m_videoDevice);
	m_textureManager = new CTextureManager(m_videoDevice);
	m_imageManager = new CImageManager(m_videoDevice);
	m_shaderManager = new CShaderManager(m_videoDevice);
}

// Called when the CoreWindow object is created (or re-created).
void UwpEngine::EngineMain::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &EngineMain::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &EngineMain::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &EngineMain::OnWindowClosed);

	// Register for keypress notifications.
	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &EngineMain::OnKeyPressed);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &EngineMain::OnKeyReleased);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &EngineMain::OnPointerPressed);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &EngineMain::OnPointerReleased);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &EngineMain::OnPointerMoved);

	window->PointerWheelChanged +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &EngineMain::OnPointerWheel);
	
	
	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &EngineMain::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &EngineMain::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &EngineMain::OnDisplayContentsInvalidated);

	m_videoDevice->SetWindow(window);

	m_mouseDevice->SetWindow(window);

	// Uncomment for FPS move
	//m_mouseDevice->HideCursor();
}

// Initializes scene resources, or loads a previously saved app state.
void UwpEngine::EngineMain::Load(Platform::String^ entryPoint)
{
	if (m_eventManager == nullptr)
	{
		m_eventManager = new EventManager(m_videoDevice, m_soundManager, m_fontManager, m_textureManager, m_imageManager, m_shaderManager);

		m_eventManager->LoadSounds();
		m_eventManager->LoadFonts();
		m_eventManager->LoadTextures();
		m_eventManager->LoadShaders();
		m_eventManager->LoadImages();
		m_eventManager->LoadHudImages();
		m_eventManager->LoadModels();
	}
}

// This method is called after the window becomes active.
void UwpEngine::EngineMain::Run()
{
	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			for (int i = 0;i < CMouseDevice::BUTTON_COUNT;i++)
			{
				if (m_mouseDevice->m_keyMap[i]->m_isDown)
				{
					if (m_mouseDevice->m_keyMap[CMouseDevice::RMB]->m_isDown)
					{
						// When RMB down MMO move is a regular first person shooter mouse move
						m_mouseDevice->MouseMove();

						CPacket p;
						p.m_type = CPacket::PT_FPSMOVE;
						memcpy(&p.m_data[0], &m_mouseDevice->lY, 4);
						memcpy(&p.m_data[4], &m_mouseDevice->lX, 4);
						memcpy(&p.m_data[8], &m_mouseDevice->lZ, 4);
						memcpy(&p.m_data[12], &m_mouseDevice->m_sensitivity, 4);

						m_eventManager->SendEvent(&p);

						m_mouseDevice->FirstPersonReset();
					}
					else
					{
						m_eventManager->SendEvent(m_mouseDevice->SetState(i));
					}
				}
			}

			for (int i = 0; i < CKeyboardDevice::KEY_COUNT; i++)
			{
				if (m_keyboardDevice->m_keyMap[i]->m_isDown)
				{
					m_eventManager->SendEvent(m_keyboardDevice->SetState(i));
				}
			}

			m_eventManager->Update();

			if (m_eventManager->Render())
			{
				m_videoDevice->Present();
			}
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void UwpEngine::EngineMain::Uninitialize()
{
}

// Application lifecycle event handlers.

void UwpEngine::EngineMain::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void UwpEngine::EngineMain::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		m_videoDevice->Trim();

		// Insert your code here.

		deferral->Complete();
	});
}

void UwpEngine::EngineMain::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// Insert your code here.
}

// Window event handlers.

void UwpEngine::EngineMain::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_videoDevice->SetLogicalSize(Windows::Foundation::Size(sender->Bounds.Width, sender->Bounds.Height));
	m_eventManager->CreateWindowSizeDependentResources();
}

void UwpEngine::EngineMain::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void UwpEngine::EngineMain::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void UwpEngine::EngineMain::OnKeyPressed(CoreWindow^ sender, KeyEventArgs^ args)
{
	m_keyboardDevice->m_keyMap[(int)args->VirtualKey]->m_isDown = true;
}

void UwpEngine::EngineMain::OnKeyReleased(CoreWindow^ sender, KeyEventArgs^ args)
{
	m_keyboardDevice->ResetKey((int)args->VirtualKey);
}

void UwpEngine::EngineMain::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	if (args->CurrentPoint->Properties->IsLeftButtonPressed)
	{
		m_mouseDevice->m_keyMap[CMouseDevice::LMB]->m_isDown = true;
	}

	if (args->CurrentPoint->Properties->IsMiddleButtonPressed)
	{
		m_mouseDevice->m_keyMap[CMouseDevice::MMB]->m_isDown = true;
	}

	if (args->CurrentPoint->Properties->IsRightButtonPressed)
	{
		m_mouseDevice->m_keyMap[CMouseDevice::RMB]->m_isDown = true;

		// Checking for MMO style mouse move
		if (m_mouseDevice->m_keyMap[CMouseDevice::RMB]->m_isDown)
		{
			m_mouseDevice->HideCursor();

			m_mouseDevice->SaveLastPosition();

			m_mouseDevice->FirstPersonReset();
		}
	}
}

void UwpEngine::EngineMain::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	m_mouseDevice->ResetButtons();

	m_mouseDevice->SetLastPosition();

	// Comment out for FPS move
	m_mouseDevice->ShowCursor();
}

void UwpEngine::EngineMain::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Uncomment this for FPS move
	/*
	m_mouseDevice->MouseMove();

	CPacket p;
	p.m_type = CPacket::PT_FPSMOVE;
	memcpy(&p.m_data[0], &m_mouseDevice->lY, 4);
	memcpy(&p.m_data[4], &m_mouseDevice->lX, 4);
	memcpy(&p.m_data[8], &m_mouseDevice->lZ, 4);
	memcpy(&p.m_data[12], &m_mouseDevice->m_sensitivity, 4);

	m_eventManager->ProcessEvent(&p);
	
	m_mouseDevice->FirstPersonReset();
	*/

	if (!m_mouseDevice->m_keyMap[CMouseDevice::RMB]->m_isDown)
	{
		m_mouseDevice->SaveLastPosition();
	}
}

void UwpEngine::EngineMain::OnPointerWheel(CoreWindow^ sender, PointerEventArgs^ args)
{
	m_mouseDevice->MouseWheel(args->CurrentPoint->Properties->MouseWheelDelta);
}


// DisplayInformation event handlers.

void UwpEngine::EngineMain::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on CVideoDevice,
	// you should always retrieve it using the GetDpi method.
	// See CVideoDevice.cpp for more details.
	m_videoDevice->SetDpi(sender->LogicalDpi);
	m_eventManager->CreateWindowSizeDependentResources();
}

void UwpEngine::EngineMain::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	m_videoDevice->SetCurrentOrientation(sender->CurrentOrientation);
	m_eventManager->CreateWindowSizeDependentResources();
}

void UwpEngine::EngineMain::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	m_videoDevice->ValidateDevice();
}