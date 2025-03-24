#include "Transform.h"
#include "HDL_ConstantBuffer.h"

Transform::Transform() :
	mPosition(XMFLOAT3(0, 0, 0)),
	mRotation(XMFLOAT3(0, 0, 0)),
	mScale   (XMFLOAT3(1, 1, 1)),
	mWorld   (XMMatrixIdentity())
{
}

Transform::~Transform()
{
	delete pCBuff_World;
}

void Transform::Init()
{
	mWorld *= XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	mWorld *= XMMatrixRotationY(mRotation.y);
	mWorld *= XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
}

void Transform::Init(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle)
{
	mWorld *= XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	mWorld *= XMMatrixRotationY(mRotation.y);
	mWorld *= XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

	UINT64 bufferSize = (sizeof(XMMATRIX) + 0xff) & ~0xff;
	pCBuff_World = new HDL_ConstantBuffer(bufferSize);
	pCBuff_World->CopyBufferToVRAM(&mWorld);
	pCBuff_World->CreateCBV(heapHandle);
}

void Transform::Update()
{
	auto scale       = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	auto rotate      = XMMatrixRotationY(mRotation.y);
	auto translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	mWorld = scale * rotate * translation;
	pCBuff_World->UpdateCBuffer(&mWorld);
}

void Transform::UpdateWithoutCBuffer()
{
	auto scale       = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	auto rotate      = XMMatrixRotationY(mRotation.y);
	auto translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	mWorld = scale * rotate * translation;
}
