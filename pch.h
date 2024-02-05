#pragma once

#define _USE_MATH_DEFINES

#include <wrl.h>
#include <wrl/client.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <ppltasks.h>
#include <wincodec.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>

#include <DirectXColors.h>
#include <DirectXMath.h>

#include <XAudio2.h>
#include <XAudio2fx.h>
#include <X3DAudio.h>
#include <xapofx.h>

#include <string>

using namespace Concurrency;

using namespace D2D1;
using namespace DirectX;

using namespace Microsoft::WRL;

using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Storage;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;

using namespace Windows::System::Diagnostics;


inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// Set a breakpoint on this line to catch Win32 API errors.
		throw Platform::Exception::CreateException(hr);
	}
}

// Function that reads from a binary file asynchronously.
inline Concurrency::task<std::vector<byte>> ReadDataAsync(const std::wstring& filename)
{
	auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

	return create_task(folder->GetFileAsync(Platform::StringReference(filename.c_str()))).then([](StorageFile^ file)
		{
			return FileIO::ReadBufferAsync(file);
		}).then([](Streams::IBuffer^ fileBuffer) -> std::vector<byte>
			{
				std::vector<byte> returnBuffer;
				returnBuffer.resize(fileBuffer->Length);
				Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<byte>(returnBuffer.data(), fileBuffer->Length));
				return returnBuffer;
			});
}

// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
inline float ConvertDipsToPixels(float dips, float dpi)
{
	static const float dipsPerInch = 96.0f;
	return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
}

#if defined(_DEBUG)
// Check for SDK Layer support.
inline bool SdkLayersAvailable()
{
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		nullptr,                    // Any feature level will do.
		0,
		D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
		nullptr,                    // No need to keep the D3D device reference.
		nullptr,                    // No need to know the feature level.
		nullptr                     // No need to keep the D3D device context reference.
	);

	return SUCCEEDED(hr);
}
#endif