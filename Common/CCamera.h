#pragma once

#include "pch.h"

#include "Common/CVideoDevice.h"

namespace UwpEngine
{
	class CCamera
	{
	public:

		XMFLOAT3		m_pos;
		XMFLOAT3		m_rot;
		XMFLOAT3		m_right;
		XMFLOAT3		m_up;
		XMFLOAT3		m_look;
		float			m_yaw;
		float			m_pitch;
		float			m_roll;
		XMMATRIX		m_wvpMat;

		CCamera();
		CCamera(CVideoDevice* videoDevice);
		~CCamera();

		void UpdateView();
		void UpdateRotation(float x, float y, float z);
		void Copy(CCamera* camera);

	private:

		CVideoDevice*	m_videoDevice;
		XMMATRIX		m_xmworld;
		XMMATRIX		m_xmview;
		XMMATRIX		m_xmproj;
	};
}