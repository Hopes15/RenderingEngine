#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class CameraComponent
{
public:
	CameraComponent();

	~CameraComponent();

	void Init(struct D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);

	void Update();

private:
	struct Matrix
	{
		XMMATRIX view;
		XMMATRIX proj;
	};

	Matrix mViewProj = {};

	class HDL_Renderer*		  pRenderer = nullptr;
	class HDL_ConstantBuffer* pCBuff_VP = nullptr;
};

