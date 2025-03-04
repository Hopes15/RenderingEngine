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
	for (auto descHeap : pDescHeaps)
	{
		delete descHeap;
	}
	delete pMesh;
}

void GameObject::Update(float deltaTime)
{
	pTransform->mRotation.y += 1.0f * deltaTime;

	pTransform->Update();
	pCam->Update();
}

void GameObject::Draw()
{
	pMeshRenderer->Draw();
}

void GameObject::LoadComponents()
{
	pMesh = new Mesh();
	pMesh->Init(L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX");
	//pMesh->Init(L"Assets/FREE-Glass_ver1_788polygon.fbx");
	//pMesh->Init(L"Assets/uploads_files_5718873_Volkswagen+Beetle+1963.fbx");

	//DescriptorHeap
	//For CBuff
	pDescHeaps[0] = new HDL_DescriptorHeap();
	pDescHeaps[0]->CreateAsCBV_SRV_UAV(2);

	auto heapHandle = pDescHeaps[0]->GetPointerOfDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//Transform
	pTransform = new Transform();
	pTransform->SetPosition(0.0f, -20.0f, 0.0f);
	pTransform->SetScale(1.0f, 1.0f, 1.0f);
	pTransform->Init(heapHandle);

	heapHandle.ptr += pRenderer->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//CamComponent
	pCam = new CameraComponent();
	pCam->Init(heapHandle);

	//MeshRenderer
	pMeshRenderer = new MeshRenderer(pDescHeaps, pMesh);
	pMeshRenderer->Init();
}
