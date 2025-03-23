#pragma once

class Volkswagen
{
public:
	Volkswagen(class Camera* cam);

	~Volkswagen();

	void Update();

	void Input();

	void Draw();

private:
	void Init();

	class HDL_DescriptorHeap* m_pDescHeap	= nullptr;
	class HDL_Input*		  m_pInput		= nullptr;
	class Transform*		  m_pTransform	= nullptr;
	class Camera*			  m_pCam		= nullptr;
	class Mesh*				  m_pMesh		= nullptr;
	class MeshRenderer*		  m_pMRendrerer = nullptr;

	//FILE
	const char* textFile = "Assets/Volkswagen/Volkswagen.txt";
};

