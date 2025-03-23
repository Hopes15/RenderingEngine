#pragma once

class Alicia
{
public:
	Alicia(class Camera* cam);

	~Alicia();

	void Update();

	void Input();

	void Draw();

private:
	void Init();

	class HDL_DescriptorHeap* m_pDescHeap   = nullptr;
	class HDL_Input*		  m_pInput      = nullptr;
	class Transform*		  m_pTransform  = nullptr;
	class Camera*		      m_pCam	    = nullptr;
	class Mesh*				  m_pMesh	    = nullptr;
	class MeshRenderer*       m_pMRendrerer = nullptr;

	float moveSpeed = 1.0f;

	//FILE
	const char* textFile = "Assets/Alicia_TGA/Alicia.txt";

	//Parameters
	float mSpeed = 0.05f;

	//Input
	bool mInput_W = false;
	bool mInput_A = false;
	bool mInput_S = false;
	bool mInput_D = false;
};

