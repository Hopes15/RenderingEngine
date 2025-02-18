#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
};

class Mesh
{
public:
	Mesh();

	~Mesh();

	void Init();

	void Draw();

public:
	std::vector<Vertex>   mVertices;
	std::vector<uint16_t> mIndices;

private:
	class HDL_VertexBuffer* pVertexBuff = nullptr;
	class HDL_IndexBuffer*  pIndexBuff  = nullptr;
	class HDL_Renderer*		pRenderer	= nullptr;
};

