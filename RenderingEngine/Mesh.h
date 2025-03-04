#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "Loader.h"

using namespace DirectX;

class Mesh
{
public:
	Mesh();

	~Mesh();

	void Init(const wchar_t* fileName);

	inline std::vector<MeshData> GetMeshDatas() { return mMeshDatas; }

	inline std::vector<class HDL_VertexBuffer*> GetVertexBuffs() { return mVertexBuffs; }

	inline std::vector<class HDL_IndexBuffer*> GetIndexBuffs() { return mIndexBuffs; }

private:
	std::vector<MeshData> mMeshDatas;
	std::vector<class HDL_VertexBuffer*> mVertexBuffs;
	std::vector<class HDL_IndexBuffer*>  mIndexBuffs;

	class HDL_Renderer*		pRenderer	= nullptr;
};

