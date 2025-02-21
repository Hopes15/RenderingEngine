#include "Mesh.h"
#include "HDL_Renderer.h"
#include "HDL_VertexBuffer.h"
#include "HDL_IndexBuffer.h"

Mesh::Mesh() : pRenderer(HDL_Renderer::GetInstance())
{
}

Mesh::~Mesh()
{
	for (auto indexBuff : mIndexBuffs)
	{
		delete indexBuff;
	}

	for (auto vertexBuff : mVertexBuffs)
	{
		delete vertexBuff;
	}
}

void Mesh::Init(const wchar_t* fileName)
{
	//Load設定
	ImportSettings settings =
	{
		fileName,
		mMeshDatas,
		false,
		true
	};

	//モデルロード
	ModelLoader modelloader(settings);


	//ロードデータからバッファの作成
	for (size_t i = 0; i < mMeshDatas.size(); i++)
	{
		//Vertex
		UINT64 size   = sizeof(Vertex) * mMeshDatas[i].vertices.size();
		UINT   stride = sizeof(Vertex);

		auto pVB      = new HDL_VertexBuffer(size, stride);
		pVB->CopyBufferToVRAM(mMeshDatas[i].vertices.data());
		mVertexBuffs.push_back(pVB);

		//Index
		size = sizeof(uint16_t) * mMeshDatas[i].indices.size();

		auto pIB = new HDL_IndexBuffer(size);
		pIB->CopyBufferToVRAM(mMeshDatas[i].indices.data());
		mIndexBuffs.push_back(pIB);
	}
}

void Mesh::Draw()
{
	//Get CmdList
	auto cmdList = pRenderer->GetCmdList();

	//Set PrimitiveTopology
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (size_t i = 0; i < mMeshDatas.size(); i++)
	{
		auto vbView = mVertexBuffs[i]->GetView();
		auto ibView = mIndexBuffs[i]->GetView();

		//Set VertexBuffer
		cmdList->IASetVertexBuffers(0, 1, vbView);

		//Set IndexBuffer
		cmdList->IASetIndexBuffer(ibView);

		//Draw
		auto indices = static_cast<UINT>(mMeshDatas[i].indices.size());
		cmdList->DrawIndexedInstanced(indices, 1, 0, 0, 0);
	}
}