#include "Volkswagen.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "HDL_DescriptorHeap.h"
#include "HDL_Renderer.h"

Volkswagen::Volkswagen(Camera* cam) :
	m_pCam(cam)
{
	Init();
}

Volkswagen::~Volkswagen()
{
	delete m_pMRendrerer;
	delete m_pMesh;
	delete m_pTransform;
	delete m_pDescHeap;
}

void Volkswagen::Update()
{
}

void Volkswagen::Input()
{
}

void Volkswagen::Draw()
{
	m_pMRendrerer->Draw();
}

void Volkswagen::Init()
{
	auto device = HDL_Renderer::GetInstance()->GetDevice();
	auto incSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//Create DescHeap WVP
	m_pDescHeap = new HDL_DescriptorHeap;
	m_pDescHeap->CreateAsCBV_SRV_UAV(256);
	auto handle = m_pDescHeap->GetPointerOfDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//Transform
	m_pTransform = new Transform;
	m_pTransform->SetRotation(0, XM_PIDIV4, 0);
	m_pTransform->SetPosition(0.0f, 0, 1.0f);
	m_pTransform->Init(handle);
	handle.ptr += incSize;

	//Cam
	m_pCam->Register(handle);
	handle.ptr += incSize;

	//Mesh
	m_pMesh = new Mesh();
	m_pMesh->Load(textFile);
	m_pMesh->Create(handle);

	//Meshrenderer
	m_pMRendrerer = new MeshRenderer(m_pMesh, m_pDescHeap);
}
