#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Transform
{
public:
	Transform();

	~Transform();

	/// <summary>
	/// None ConstantBuffer
	/// </summary>
	void Init();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(struct D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

	/// <summary>
	/// CBufferを利用しない場合の更新
	/// </summary>
	void UpdateWithoutCBuffer();

	/// <summary>
	/// 座標を設定
	/// </summary>
	inline void SetPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
	};

	/// <summary>
	/// 回転を設定
	/// </summary>
	inline void SetRotation(float x, float y, float z)
	{
		mRotation.x = x;
		mRotation.y = y;
		mRotation.z = z;
	};

	/// <summary>
	/// スケールを設定
	/// </summary>
	inline void SetScale(float x, float y, float z)
	{
		mScale.x = x;
		mScale.y = y;
		mScale.z = z;
	};

	/// <summary>
	/// 座標を取得
	/// </summary>
	inline XMFLOAT3 GetPosition()	 const { return mPosition; }

	/// <summary>
	/// 回転を取得
	/// </summary>
	inline XMFLOAT3 GetRotation()	 const { return mRotation; }

	/// <summary>
	/// スケールを取得
	/// </summary>
	inline XMFLOAT3 GetScale()       const { return mScale; }

	/// <summary>
	/// ワールド行列を返す
	/// </summary>
	inline XMMATRIX GetWorldMatrix() const { return mWorld; }

	/// <summary>
	/// Transformコンポーネントを取得
	/// </summary>
	/// <returns></returns>
	inline Transform* GetTransform()	   { return this; }

public:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

private:
	XMMATRIX mWorld;

	class HDL_ConstantBuffer* pCBuff_World = nullptr;
};

