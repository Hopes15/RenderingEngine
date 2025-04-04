#include "Alicia.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "HDL_DescriptorHeap.h"
#include "HDL_Renderer.h"
#include "HDL_Input.h"

Alicia::Alicia(Camera* cam) :
	m_pCam(cam),
	m_pInput(HDL_Input::GetInstance())
{
	float scale = 0.01f;
	Init();
}

Alicia::~Alicia()
{
	delete m_pMRendrerer;
	delete m_pMesh;
	delete m_pTransform;
	delete m_pDescHeap;
}

void Alicia::Update()
{
	if      (mInput_W) m_pTransform->mPosition.z += mSpeed;
	else if (mInput_S) m_pTransform->mPosition.z -= mSpeed;

	if      (mInput_A) m_pTransform->mPosition.x -= mSpeed;
	else if (mInput_D) m_pTransform->mPosition.x += mSpeed;

	m_pTransform->Update();
}

void Alicia::Input()
{
	mInput_W = m_pInput->GetKey(DIK_W);
	mInput_A = m_pInput->GetKey(DIK_A);
	mInput_S = m_pInput->GetKey(DIK_S);
	mInput_D = m_pInput->GetKey(DIK_D);
}

void Alicia::Draw()
{
	m_pMRendrerer->Draw();
}

void Alicia::Init()
{
	auto device  = HDL_Renderer::GetInstance()->GetDevice();
	auto incSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//Create DescHeap WVP
	m_pDescHeap = new HDL_DescriptorHeap;
	m_pDescHeap->CreateAsCBV_SRV_UAV(256);
	auto handle = m_pDescHeap->GetPointerOfDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//Transform
	m_pTransform = new Transform;
	m_pTransform->SetRotation(0, 3.14f, 0);
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
