#include "MeshRenderer.h"
#include "Mesh.h"
#include "HDL_DescriptorHeap.h"
#include "HDL_RootSignature.h"
#include "HDL_PSO.h"
#include "HDL_Renderer.h"

MeshRenderer::MeshRenderer(Mesh* mesh, HDL_DescriptorHeap* descHeap) :
	m_pMesh(mesh),
	m_pDescHeap(descHeap),
	m_pRenderer(HDL_Renderer::GetInstance())
{
	CD3DX12_DESCRIPTOR_RANGE descRanges[4] = {};
	UINT b0 = 0;
	UINT b1 = 1;
	UINT b2 = 2;
	UINT t0 = 0;
	descRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, b0); //WorldMat
	descRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, b1); //ViewProjMat
	descRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, b2); //Material
	descRanges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, t0); //Texture

	CD3DX12_ROOT_PARAMETER rootParams[2] = {};
	rootParams[0].InitAsDescriptorTable(2, &descRanges[0]);
	rootParams[1].InitAsDescriptorTable(2, &descRanges[2]);

	D3D12_STATIC_SAMPLER_DESC sampler[1] = {};
	sampler[0].Filter			= D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler[0].AddressU			= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler[0].AddressV			= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler[0].AddressW			= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler[0].BorderColor		= D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler[0].MaxLOD			= D3D12_FLOAT32_MAX;
	sampler[0].MinLOD			= 0.0f;
	sampler[0].ComparisonFunc	= D3D12_COMPARISON_FUNC_NEVER;
	sampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
	rootSigDesc.Flags				= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSigDesc.pParameters			= rootParams;
	rootSigDesc.NumParameters		= _countof(rootParams);
	rootSigDesc.pStaticSamplers		= sampler;
	rootSigDesc.NumStaticSamplers	= _countof(sampler);

	m_pRootSig = new HDL_RootSignature(&rootSigDesc);
	m_pRootSig->CreateRootSignature();

	D3D12_INPUT_ELEMENT_DESC elements[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	D3D12_INPUT_LAYOUT_DESC inputLayout = {};
	inputLayout.pInputElementDescs = elements;
	inputLayout.NumElements		   = _countof(elements);

	m_pPSO = new HDL_PSO(inputLayout, m_pRootSig->GetPointerOfRootSignature());
	m_pPSO->SetVS(L"Model_VS.hlsl");
	m_pPSO->SetPS(L"Model_PS.hlsl");
	m_pPSO->CreatePipeline();
}

MeshRenderer::~MeshRenderer()
{
	delete m_pPSO;
	delete m_pRootSig;
}

void MeshRenderer::Draw()
{
	auto cmdList = m_pRenderer->GetCmdList();

	//Set Pipeline
	cmdList->SetPipelineState(m_pPSO->GetPointerOfPipeline());

	//Set RootSignature
	cmdList->SetGraphicsRootSignature(m_pRootSig->GetPointerOfRootSignature());

	//Set DescriptorHeap
	ID3D12DescriptorHeap* pDescHeaps[] = 
	{ 
		m_pDescHeap[0].GetPointerOfDescriptorHeap(),
		m_pDescHeap[1].GetPointerOfDescriptorHeap()
	};
	cmdList->SetDescriptorHeaps(1, pDescHeaps);

	//DescHeap 0
	auto heapHandle = pDescHeaps[0]->GetGPUDescriptorHandleForHeapStart();

	//WVP Matrix
	cmdList->SetGraphicsRootDescriptorTable(0, heapHandle);

	//メッシュの描画
	auto& meshData    = m_pMesh->GetMeshData();
	auto& vertexBuffs = m_pMesh->GetVertexBuffs();
	auto& texBuffs	  = m_pMesh->GetTextureBuffs();
	auto  size		  = m_pMesh->GetNumParts();

	//PrimitiveTopologyをセット
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DescHeap 1
	heapHandle = pDescHeaps[1]->GetGPUDescriptorHandleForHeapStart();
	auto incSize = m_pRenderer->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 2;

	for (UINT i = 0; i < size; i++)
	{
		//VertexBufferをセット
		auto vbv = vertexBuffs[i]->GetView();
		cmdList->IASetVertexBuffers(0, 1, vbv);

		//Set Materials
		cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);
		heapHandle.ptr += incSize;

		texBuffs[i]->OpenTextureBuffer();

		//Draw
		cmdList->DrawInstanced(meshData[i].numVertices, 1, 0, 0);

		texBuffs[i]->CloseTextureBuffer();
	}
}
