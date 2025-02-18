#pragma once

class GameObject
{
public:
	GameObject();

	~GameObject();

	void Update();

	void Draw();

private:
	void LoadComponents();

private:
	const unsigned int NUM_VIEWS;

	class Mesh*				  pMesh			= nullptr;
	class Transform*		  pTransform	= nullptr;
	class CameraComponent*	  pCam			= nullptr;
	class MeshRenderer*		  pMeshRenderer = nullptr;
	class HDL_DescriptorHeap* pDescHeap     = nullptr;
	class HDL_Renderer*		  pRenderer		= nullptr;
};

