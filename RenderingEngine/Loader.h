#pragma once
#include "CommonDX12Header.h"
#include <vector>
#include <string>
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	XMFLOAT3 tangent;
	XMFLOAT4 color;
};

struct MeshData
{
	std::vector<Vertex>   vertices;
	std::vector<uint16_t> indices;
	std::wstring		  diffuseMap;
};

struct ImportSettings
{
	const wchar_t*	       fileName = nullptr;
	std::vector<MeshData>& meshes;
	bool				   inverseU = false;
	bool				   inverseV = false;
};

class ModelLoader
{
public:
	ModelLoader(ImportSettings settings);

private:
	void LoadMesh(MeshData& dst, const struct aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filePath, MeshData& dst, const struct aiMaterial* src);
};

