#include "Demo.h"
#include "GameSystem.h"
#include "Transform.h"
#include "CameraComponent.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "HDL_DescriptorHeap.h"
#include "HDL_Renderer.h"
#include "Converter.h"

Demo::Demo()
{
	float scale = 0.01f;

#if 0
	//コンバートする時だけ
	Converter converter;
	converter.ConvertFBX(fileName, textFile, scale, scale, scale, 0, 1, 2);
#endif

	Init();
}

Demo::~Demo()
{
	delete m_pMRendrerer;
	delete m_pMesh;
	delete m_pCam;
	delete m_pTransform;
	delete[] m_pDescHeap;
}

void Demo::Update()
{
	radian += 0.01f;
	erapsed += sinf(radian) * 0.1f;

	m_pTransform->SetRotation(0.0f, radian, 0.0f);
	m_pTransform->Update();
}

void Demo::Draw()
{
	m_pMRendrerer->Draw();
}

void Demo::Init()
{
	auto device  = HDL_Renderer::GetInstance()->GetDevice();
	auto incSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//Create DescHeap WVP
	m_pDescHeap = new HDL_DescriptorHeap[2];
	m_pDescHeap[0].CreateAsCBV_SRV_UAV(2);
	auto handle = m_pDescHeap[0].GetPointerOfDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//Transform
	m_pTransform = new Transform;
	//m_pTransform->SetScale(1.5f, 1.5f, 1.5f);
	m_pTransform->Init(handle);
	handle.ptr += incSize;
	 
	//Cam
	m_pCam = new CameraComponent;
	m_pCam->Init(handle);

	//Create DescHeap MESH
	m_pDescHeap[1].CreateAsCBV_SRV_UAV(256);
	handle = m_pDescHeap[1].GetPointerOfDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();

	//Mesh
	m_pMesh = new Mesh();
	m_pMesh->Load(textFile);
	m_pMesh->Create(handle);

	//Meshrenderer
	m_pMRendrerer = new MeshRenderer(m_pMesh, m_pDescHeap);
}
