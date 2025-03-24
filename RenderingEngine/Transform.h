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
	/// ������
	/// </summary>
	void Init(struct D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// CBuffer�𗘗p���Ȃ��ꍇ�̍X�V
	/// </summary>
	void UpdateWithoutCBuffer();

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	inline void SetPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
	};

	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	inline void SetRotation(float x, float y, float z)
	{
		mRotation.x = x;
		mRotation.y = y;
		mRotation.z = z;
	};

	/// <summary>
	/// �X�P�[����ݒ�
	/// </summary>
	inline void SetScale(float x, float y, float z)
	{
		mScale.x = x;
		mScale.y = y;
		mScale.z = z;
	};

	/// <summary>
	/// ���W���擾
	/// </summary>
	inline XMFLOAT3 GetPosition()	 const { return mPosition; }

	/// <summary>
	/// ��]���擾
	/// </summary>
	inline XMFLOAT3 GetRotation()	 const { return mRotation; }

	/// <summary>
	/// �X�P�[�����擾
	/// </summary>
	inline XMFLOAT3 GetScale()       const { return mScale; }

	/// <summary>
	/// ���[���h�s���Ԃ�
	/// </summary>
	inline XMMATRIX GetWorldMatrix() const { return mWorld; }

	/// <summary>
	/// Transform�R���|�[�l���g���擾
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

