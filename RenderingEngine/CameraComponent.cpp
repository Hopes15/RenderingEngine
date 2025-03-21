#include "CameraComponent.h"
#include "HDL_ConstantBuffer.h"
#include "HDL_Renderer.h"

CameraComponent::CameraComponent() :
	pRenderer(HDL_Renderer::GetInstance())
{
}

CameraComponent::~CameraComponent()
{
	delete pCBuff_VP;
}

void CameraComponent::Init(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle)
{
	//ViewProjectionçsóÒ(3D)
	float h = 1.35f;
	XMFLOAT3 eye    = { 0, h, -2.4f };
	XMVECTOR target = { 0, h, 0 };
	XMVECTOR up     = { 0, 1, 0 };

	//ç¿ïWånÇÃèâä˙âª
	mViewProj.view = XMMatrixLookAtLH(XMLoadFloat3(&eye), target, up);
	mViewProj.proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(pRenderer->WINDOW_WIDTH) / static_cast<float>(pRenderer->WINDOW_HEIGHT),
		0.5f,   //near
		1000.0f //far
	);
	mViewProj.eye = eye;

	//CBuffer
	UINT64 bufferSize = (sizeof(XMMATRIX) + 0xff) & ~0xff;
	pCBuff_VP = new HDL_ConstantBuffer(bufferSize, heapHandle);
	pCBuff_VP->CopyBufferToVRAM(&mViewProj);
}

void CameraComponent::Update()
{
}
