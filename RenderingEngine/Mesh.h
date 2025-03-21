#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "HDL_VertexBuffer.h"
#include "HDL_IndexBuffer.h"
#include "HDL_ConstantBuffer.h"
#include "HDL_Texture.h"

using namespace DirectX;

class Mesh
{
public:
	Mesh();

	~Mesh();

	void Load(const char* fileName);

	void Create(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);

private:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	struct Material
	{
		XMFLOAT4	ambient;
		XMFLOAT4	diffuse;
		XMFLOAT4	specular;
	};

	struct MeshData
	{
		UINT				  numVertices;
		std::vector<Vertex>   vertices;
		Material		      material;
		std::wstring		  fileName;
	};

	UINT mNumParts;

	std::vector<MeshData>			 mMeshData = {};
	std::vector<HDL_VertexBuffer*>   mVertexBuffs;
	std::vector<HDL_ConstantBuffer*> mCBuffs;
	std::vector<HDL_TextureBuffer*>  mTexBuffs;

public:
	UINT GetNumParts() const { return mNumParts; }

	std::vector<MeshData>&           GetMeshData()	   { return mMeshData; }
	std::vector<HDL_VertexBuffer*>&  GetVertexBuffs()  { return mVertexBuffs; }
	std::vector<HDL_TextureBuffer*>& GetTextureBuffs() { return mTexBuffs; }
};

