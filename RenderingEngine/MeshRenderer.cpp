#include "MeshRenderer.h"
#include "HDL_RootSignature.h"
#include "HDL_PSO.h"
#include "HDL_Renderer.h"
#include "HDL_DescriptorHeap.h"
#include "HDL_VertexBuffer.h"
#include "HDL_IndexBuffer.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer(HDL_DescriptorHeap** descHeap, class Mesh* mesh) :
	pRenderer(HDL_Renderer::GetInstance()),
	pDescHeap(descHeap),
	pMesh(mesh)
{
}

MeshRenderer::~MeshRenderer()
{
	delete pPSO;
	delete pRootSig;
}

void MeshRenderer::Init()
{
	//DescriptorRange
	CD3DX12_DESCRIPTOR_RANGE descRanges[3] = {};
	descRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, /*NumDescriptor = */ 1, /*slot = */ 0);
	descRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, /*NumDescriptor = */ 1, /*slot = */ 1);

	//RootParameter
	CD3DX12_ROOT_PARAMETER rootParams[3] = {};
	rootParams[0].InitAsDescriptorTable(/*NumDescRanges = */ 2, &descRanges[0]);


	//Sampler
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc[1] = {};
	samplerDesc[0].Init(/*slot = */ 0);

	//RootSignatureDesc
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
	rootSigDesc.pParameters		  = rootParams;
	rootSigDesc.NumParameters	  = _countof(rootParams);
	rootSigDesc.pStaticSamplers   = samplerDesc;
	rootSigDesc.NumStaticSamplers = _countof(samplerDesc);
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートシグネチャを作成
	pRootSig = new HDL_RootSignature(&rootSigDesc);
	pRootSig->CreateRootSignature();

	//インプットレイアウトを作成
	D3D12_INPUT_ELEMENT_DESC elementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT"	, 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_INPUT_LAYOUT_DESC inputLayout = {};
	inputLayout.pInputElementDescs = elementDesc;
	inputLayout.NumElements		   = _countof(elementDesc);

	//PSOを作成
	pPSO = new HDL_PSO(inputLayout, pRootSig->GetPointerOfRootSignature());
	//TODO:読み取ったモデルデータからシェーダーを指定できるようにする
	pPSO->SetVS(L"GameObject_VS.hlsl");
	pPSO->SetPS(L"GameObject_PS.hlsl");
	pPSO->CreatePipeline();
}

void MeshRenderer::Draw()
{
	//Get CmdList
	auto cmdList = pRenderer->GetCmdList();
	auto meshDatas = pMesh->GetMeshDatas();

	//Set Pipeline, RootSignature
	cmdList->SetPipelineState(pPSO->GetPointerOfPipeline());
	cmdList->SetGraphicsRootSignature(pRootSig->GetPointerOfRootSignature());

	//Set PrimitiveTopology
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set DescriptorHeap
	//WVP
	cmdList->SetDescriptorHeaps(1, pDescHeap[0]->GetAddressOfDescriptorHeap());

	auto heapHandle = pDescHeap[0]->GetPointerOfDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(0, heapHandle);

	auto vertBuffs  = pMesh->GetVertexBuffs();
	auto indexBuffs = pMesh->GetIndexBuffs();

	for (size_t i = 0; i < meshDatas.size(); i++)
	{
		auto vbView = vertBuffs[i]->GetView();
		auto ibView = indexBuffs[i]->GetView();

		//Set VertexBuffer
		cmdList->IASetVertexBuffers(0, 1, vbView);

		//Set IndexBuffer
		cmdList->IASetIndexBuffer(ibView);

		//Draw
		auto indices = static_cast<UINT>(meshDatas[i].indices.size());
		cmdList->DrawIndexedInstanced(indices, 1, 0, 0, 0);
	}
}
