#include "Material.h"
#include "HDL_Texture.h"
#include "HDL_Renderer.h"
#include "Utilities.h"
#include <filesystem>

namespace fs = std::filesystem;

std::wstring ReplaceExtension(const std::wstring& src, const char* ext)
{
	fs::path p = src;
	return p.replace_extension(ext).c_str();
}


Material::Material(std::vector<MeshData>& meshDatas, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle) :
	pRenderer(HDL_Renderer::GetInstance())
{
	std::cout << "メッシュの数：" << meshDatas.size() << std::endl;

	for (size_t i = 0; i < meshDatas.size(); i++)
	{
		//拡張子をtgaに変更
		auto path = ReplaceExtension(meshDatas[i].diffuseMap, "tga");
		std::cout << ConvertWString(path) << std::endl;

		//テクスチャロード
		HDL_Texture tex(path.c_str());
		auto img = tex.GetScratchImg().GetImage(0, 0, 0);

		//テクスチャバッファ作成
		auto texBuff = new HDL_TextureBuffer(tex.GetMetaData(), img, heapHandle);
		mTexBuffs.push_back(texBuff);

		//ポインタを移動
		heapHandle.ptr += pRenderer->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
}

Material::~Material()
{
	for (auto texBuff : mTexBuffs)
	{
		delete texBuff;
	}
}
