#include "pch.h"

#include "Common/CCamera.h"

UwpEngine::CCamera::CCamera()
{
	memset(this, 0x00, sizeof(CCamera));
}

UwpEngine::CCamera::CCamera(CVideoDevice* videoDevice)
{
	memset(this, 0x00, sizeof(CCamera));

	m_videoDevice = videoDevice;

	m_xmworld = XMMatrixIdentity();

	//XMStoreFloat4x4(&xmfworld, xmworld);

	Windows::Foundation::Size outputSize = m_videoDevice->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 90.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	m_xmproj = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 1.0F, 50000.0F);

	//XMStoreFloat4x4(&xmfproj, xmproj);
}

UwpEngine::CCamera::~CCamera()
{
}

void UwpEngine::CCamera::UpdateRotation(float x, float y, float z)
{
	m_rot.x += x;

	if (m_rot.x < 0.0F)
	{
		m_rot.x = 359.9F;
	}
	else if (m_rot.x > 359.9F)
	{
		m_rot.x = 0.0F;
	}

	m_rot.y += y;

	if (m_rot.y < 0.0F)
	{
		m_rot.y = 359.9F;
	}
	else if (m_rot.y > 359.9F)
	{
		m_rot.y = 0.0F;
	}

	m_rot.z += z;

	if (m_rot.z < 0.0F)
	{
		m_rot.z = 359.9F;
	}
	else if (m_rot.z > 359.9F)
	{
		m_rot.z = 0.0F;
	}
}

void UwpEngine::CCamera::UpdateView()
{
	m_yaw	= m_rot.y * (float)(XM_PI / 180.0F);
	m_pitch = m_rot.x * (float)(XM_PI / 180.0F);
	m_roll  = m_rot.z * (float)(XM_PI / 180.0F);

	XMMATRIX xmMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);

	XMFLOAT3 xmflook;
	xmflook.x = 0.0F;
	xmflook.y = 0.0F;
	xmflook.z = 1.0F;

	XMVECTOR xmvlook = XMLoadFloat3(&xmflook);

	xmvlook = XMVector3TransformCoord(xmvlook, xmMatrix);

	xmflook = XMFLOAT3(XMVectorGetX(xmvlook), XMVectorGetY(xmvlook), XMVectorGetZ(xmvlook));


	xmflook.x = xmflook.x + m_pos.x;
	xmflook.y = xmflook.y + m_pos.y;
	xmflook.z = xmflook.z + m_pos.z;

	xmvlook = XMLoadFloat3(&xmflook);

	XMFLOAT3 xmfup;
	xmfup.x = 0.0F;
	xmfup.y = 1.0F;
	xmfup.z = 0.0F;

	XMVECTOR xmvup = XMLoadFloat3(&xmfup);

	xmvup = XMVector3TransformCoord(xmvup, xmMatrix);

	xmfup = XMFLOAT3(XMVectorGetX(xmvup), XMVectorGetY(xmvup), XMVectorGetZ(xmvup));

	
	XMVECTOR xmvposition = XMLoadFloat3(&m_pos);

	
	m_xmview = XMMatrixLookAtLH(xmvposition, xmvlook, xmvup);

	XMFLOAT4X4  xmfview;
	XMStoreFloat4x4(&xmfview, m_xmview);


	m_right	= XMFLOAT3(xmfview.m[0][0], xmfview.m[1][0], xmfview.m[2][0]);
    m_up    = XMFLOAT3(xmfview.m[0][1], xmfview.m[1][1], xmfview.m[2][1]);
	m_look	= XMFLOAT3(xmfview.m[0][2], xmfview.m[1][2], xmfview.m[2][2]);


	XMMATRIX twvpMat = m_xmworld * m_xmview * m_xmproj;

	m_wvpMat = XMMatrixTranspose(twvpMat);
}

void UwpEngine::CCamera::Copy(CCamera* c)
{
	m_xmworld = c->m_xmworld;
	m_xmproj = c->m_xmproj;
	m_pos = c->m_pos;
	m_rot = c->m_rot;

	UpdateView();
}
