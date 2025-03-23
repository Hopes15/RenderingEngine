#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();

	~Camera();

	void Init();

	void Input();

	void Update();

	void Register(struct D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);

private:
	struct Matrix
	{
		XMMATRIX view;
		XMMATRIX proj;
		XMFLOAT3 eye;
	};

	Matrix mViewProj = {};
	XMFLOAT3 mEye	 = {};
	XMVECTOR mTarget = {};
	XMVECTOR mUp     = {};

	class HDL_Renderer*		  pRenderer = nullptr;
	class Transform*		  pTrans	= nullptr;
	class HDL_ConstantBuffer* pCBuff_VP = nullptr;

	//Input
	class HDL_Input*		  m_pInput      = nullptr;
	float mSpeed = 0.05f;
	bool mInput_W = false;
	bool mInput_A = false;
	bool mInput_S = false;
	bool mInput_D = false;
};

