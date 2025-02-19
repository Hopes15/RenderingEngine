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

void GameObject::Update(float deltaTime)
{
	pTransform->mRotation.y += 1.0f * deltaTime;

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
	pMesh = new Mesh();
	pMesh->Init(L"Assets/Alicia/FBX/Alicia_solid_Unity.FBX");

	//DescriptorHeap
	pDescHeap = new HDL_DescriptorHeap();
	pDescHeap->CreateAsCBV_SRV_UAV(2);

	auto heapHandle = pDescHeap->GetPointerOfDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//Transform
	pTransform = new Transform();
	pTransform->SetScale(0.5f, 0.5f, 0.5f);
	pTransform->Init(heapHandle);

	heapHandle.ptr += pRenderer->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//CamComponent
	pCam = new CameraComponent();
	pCam->Init(heapHandle);

	//MeshRenderer
	pMeshRenderer = new MeshRenderer(pDescHeap);
	pMeshRenderer->Init();
}
