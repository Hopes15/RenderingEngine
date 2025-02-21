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
	XMFLOAT3 pos   (0, 120, -150);
	XMFLOAT3 target(0, 70, 0);
	XMFLOAT3 up    (0, 1, 0);

	//ç¿ïWånÇÃèâä˙âª
	mViewProj.view = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
	mViewProj.proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,
		static_cast<float>(pRenderer->WINDOW_WIDTH) / static_cast<float>(pRenderer->WINDOW_HEIGHT),
		0.5f,   //near
		1000.0f //far
	);

	//CBuffer
	UINT64 bufferSize = (sizeof(XMMATRIX) + 0xff) & ~0xff;
	pCBuff_VP = new HDL_ConstantBuffer(bufferSize, heapHandle);
	pCBuff_VP->CopyBufferToVRAM(&mViewProj);
}

void CameraComponent::Update()
{
}
