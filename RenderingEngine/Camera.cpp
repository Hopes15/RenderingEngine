#include "Camera.h"
#include "Transform.h"
#include "HDL_ConstantBuffer.h"
#include "HDL_Renderer.h"
#include "HDL_Input.h"

Camera::Camera() :
	pRenderer(HDL_Renderer::GetInstance()),
	m_pInput(HDL_Input::GetInstance())
{
}

Camera::~Camera()
{
	delete pCBuff_VP;
	delete pTrans;
}

void Camera::Init()
{
	pTrans = new Transform;
	pTrans->SetPosition(3.0f, 3.0f, -3.0f);
	pTrans->Init();

	//ViewProjections—ñ(3D)
	mEye    = pTrans->GetPosition();
	mTarget = { 0, 0, 0 };
	mUp     = { 0, 1, 0 };

	//À•WŒn‚Ì‰Šú‰»
	mViewProj.view = XMMatrixLookAtLH(XMLoadFloat3(&mEye), mTarget, mUp);
	mViewProj.proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(pRenderer->WINDOW_WIDTH) / static_cast<float>(pRenderer->WINDOW_HEIGHT),
		0.3f,   //near
		1000.0f //far
	);
	mViewProj.eye  = mEye;

	//CBuffer
	UINT64 bufferSize = (sizeof(XMMATRIX) + 0xff) & ~0xff;
	pCBuff_VP = new HDL_ConstantBuffer(bufferSize);
	pCBuff_VP->CopyBufferToVRAM(&mViewProj);
}

void Camera::Input()
{
	mInput_W = m_pInput->GetKey(DIK_W);
	mInput_A = m_pInput->GetKey(DIK_A);
	mInput_S = m_pInput->GetKey(DIK_S);
	mInput_D = m_pInput->GetKey(DIK_D);
}

void Camera::Update()
{
	if      (mInput_W) pTrans->mPosition.z += mSpeed;
	else if (mInput_S) pTrans->mPosition.z -= mSpeed;

	if      (mInput_A) pTrans->mPosition.x -= mSpeed;
	else if (mInput_D) pTrans->mPosition.x += mSpeed;

	pTrans->UpdateWithoutCBuffer();

	mEye    = pTrans->GetPosition();
	mTarget = { 0, pTrans->GetPosition().y, 0 };

	mViewProj.view = XMMatrixLookAtLH(XMLoadFloat3(&mEye), mTarget, mUp);
	mViewProj.eye  = mEye;
	
	//CBuffer
	pCBuff_VP->CopyBufferToVRAM(&mViewProj);
}

void Camera::Register(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle)
{
	pCBuff_VP->CreateCBV(heapHandle);
}
