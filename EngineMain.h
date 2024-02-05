#pragma once

#include "pch.h"

#include "EventManager.h"

#include "Common/CVideoDevice.h"
#include "Common/CSoundDevice.h"
#include "Common/CMouseDevice.h"
#include "Common/CKeyboardDevice.h"
#include "Common/CCamera.h"

namespace UwpEngine
{
	// Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
	ref class EngineMain sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	public:
		
		EngineMain();

		// IFrameworkView Methods
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
		virtual void Load(Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

	protected:
		
		// Application lifecycle event handlers
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
		void OnResuming(Platform::Object^ sender, Platform::Object^ args);

		// Window event handlers
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

		// DisplayInformation event handlers
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

		// Keyboard and Mouse Events
		void OnKeyPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
		void OnKeyReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
		void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerWheel(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);

	private:

		bool m_windowClosed;
		bool m_windowVisible;
		
		EventManager*		m_eventManager;
		CVideoDevice*		m_videoDevice;
		CSoundDevice*		m_soundDevice;
		CSoundManager*		m_soundManager;
		CMouseDevice*		m_mouseDevice;
		CKeyboardDevice*	m_keyboardDevice;
		CFontManager*		m_fontManager;
		CTextureManager*	m_textureManager;
		CImageManager*		m_imageManager;
		CShaderManager*		m_shaderManager;
		CCamera*			m_camera;
	};
}

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};