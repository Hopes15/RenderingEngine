#pragma once
#include <cmath>

class Demo
{
public:
	Demo();

	~Demo();

	void Update();

	void Draw();

private:
	void Init();

	class HDL_DescriptorHeap* m_pDescHeap   = nullptr;
	class Transform*		  m_pTransform  = nullptr;
	class CameraComponent*    m_pCam	    = nullptr;
	class Mesh*				  m_pMesh	    = nullptr;
	class MeshRenderer*       m_pMRendrerer = nullptr;

	float erapsed = 0.0f;
	float radian  = 0.0f;

	//const char* fileName = "Assets/Alicia/FBX/Alicia_solid_Unity.FBX";
	const char* textFile = "Assets/Alicia_TGA/Alicia.txt";
	//const char* fileName = "Assets/Volkswagen/uploads_files_5718873_Volkswagen+Beetle+1963.fbx";
	//const char* textFile = "Assets/Volkswagen/Volkswagen.txt";
};

