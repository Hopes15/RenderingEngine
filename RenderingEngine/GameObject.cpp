#include "GameObject.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "CameraComponent.h"
#include "HDL_DescriptorHeap.h"
#include "HDL_Renderer.h"

GameObject::GameObject() :
	pRenderer(HDL_Renderer::GetInstance()),
	NUM_VIEWS(2)
{
	LoadComponents();
}

GameObject::~GameObject()
{
	delete pMeshRenderer;
	delete pCam;
	delete pTransform;
	delete pDescHeap;
	delete pMesh;
}

void GameObject::Update()
{
	pTransform->mRotation.y += 0.01f;

	pTransform->Update();
	pCam->Update();
}

void GameObject::Draw()
{
	pMeshRenderer->Draw();
	pMesh->Draw();
}

void GameObject::LoadComponents()
{
	//Mesh
	Vertex vertices[] =
	{
		{{ -1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, //¶‰º
		{{ -1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}}, //¶ã
		{{  1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, //‰E‰º
		{{  1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}}  //‰Eã
	};

	uint16_t indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	pMesh = new Mesh();
	pMesh->mVertices = std::vector<Vertex>(std::begin(vertices), std::end(vertices));
	pMesh->mIndices  = std::vector<uint16_t>(std::begin(indices), std::end(indices));
	pMesh->Init();

	//DescriptorHeap
	pDescHeap = new HDL_DescriptorHeap();
	pDescHeap->CreateAsCBV_SRV_UAV(2);

	auto heapHandle = pDescHeap->GetPointerOfDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//Transform
	pTransform = new Transform();
	pTransform->SetScale(2.0f, 2.0f, 2.0f);
	pTransform->Init(heapHandle);

	heapHandle.ptr += pRenderer->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//CamComponent
	pCam = new CameraComponent();
	pCam->Init(heapHandle);

	//MeshRenderer
	pMeshRenderer = new MeshRenderer(pDescHeap);
	pMeshRenderer->Init();
}
