#pragma once
#include "Loader.h"
#include <vector>

class Material
{
public:
	Material(std::vector<MeshData>& meshDatas, struct D3D12_CPU_DESCRIPTOR_HANDLE heapHandle);

	~Material();

private:
	std::vector<class HDL_TextureBuffer*> mTexBuffs;
	class HDL_Renderer* pRenderer = nullptr;
};

