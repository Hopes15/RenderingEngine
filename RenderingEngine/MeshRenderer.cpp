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
	//ROOT SIGNATURE
	{
		//Descriptor Ranges
		CD3DX12_DESCRIPTOR_RANGE descRanges[3] = {};
		descRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, /*numDescs = */ 2, /*slot = */ 0); //WVP Mat
		descRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, /*numDescs = */ 1, /*slot = */ 2); //Material
		descRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, /*numDescs = */ 1, /*slot = */ 0); //Texture

		//RootParameters
		CD3DX12_ROOT_PARAMETER rootParams[2] = {};
		rootParams[0].InitAsDescriptorTable(/*numRanges = */ 1, &descRanges[0]);
		rootParams[1].InitAsDescriptorTable(/*numRanges = */ 2, &descRanges[1]);

		//Sampler
		D3D12_STATIC_SAMPLER_DESC sampler[1] = {};
		sampler[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler[0].MaxLOD = D3D12_FLOAT32_MAX;
		sampler[0].MinLOD = 0.0f;
		sampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		//RootSignature
		D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
		rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSigDesc.pParameters = rootParams;
		rootSigDesc.NumParameters = _countof(rootParams);
		rootSigDesc.pStaticSamplers = sampler;
		rootSigDesc.NumStaticSamplers = _countof(sampler);

		m_pRootSig = new HDL_RootSignature(&rootSigDesc);
		m_pRootSig->CreateRootSignature();
	}

	//PSO
	{
		//Input Layout
		D3D12_INPUT_ELEMENT_DESC elements[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		D3D12_INPUT_LAYOUT_DESC inputLayout = {};
		inputLayout.pInputElementDescs = elements;
		inputLayout.NumElements = _countof(elements);

		//Pipeline State
		m_pPSO = new HDL_PSO(inputLayout, m_pRootSig->GetPointerOfRootSignature());
		m_pPSO->SetVS(L"Model_VS.hlsl");
		m_pPSO->SetPS(L"Model_PS.hlsl");
		m_pPSO->CreatePipeline();
	}
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
	ID3D12DescriptorHeap* pDescHeaps[] = { m_pDescHeap->GetPointerOfDescriptorHeap() };
	cmdList->SetDescriptorHeaps(1, pDescHeaps);

	//WVP Matrix
	auto heapHandle = pDescHeaps[0]->GetGPUDescriptorHandleForHeapStart();
	cmdList->SetGraphicsRootDescriptorTable(0, heapHandle);

	//メッシュの描画
	auto& meshData	  = m_pMesh->GetMeshData();
	auto& vertexBuffs = m_pMesh->GetVertexBuffs();
	auto& texBuffs    = m_pMesh->GetTextureBuffs();
	auto  size        = m_pMesh->GetNumParts();

	//PrimitiveTopologyをセット
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto incSize = m_pRenderer->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 2;

	for (UINT i = 0; i < size; i++)
	{
		//VertexBufferをセット
		auto vbv = vertexBuffs[i]->GetView();
		cmdList->IASetVertexBuffers(0, 1, vbv);

		//Set Materials
		heapHandle.ptr += incSize;
		cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);

		texBuffs[i]->OpenTextureBuffer();

		//Draw
		cmdList->DrawInstanced(meshData[i].numVertices, 1, 0, 0);

		texBuffs[i]->CloseTextureBuffer();
	}
}
