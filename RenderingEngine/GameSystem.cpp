#include "GameSystem.h"
#include "HDL_Renderer.h"
#include "HDL_Window.h"
#include "HDL_BackBuffer.h"
#include "HDL_DepthStencilBuffer.h"
#include "HDL_Input.h"
#include "Demo.h"

GameSystem::GameSystem() :
	WIDTH(1920),
	HEIGHT(1080),
	FIXED_DELTA_TIME(0.02f),
	mActivation(true)
{
}

GameSystem::~GameSystem()
{
	delete demo;
	pInput->Destroy();
	delete pDSBuff;
	delete pBackBuff;
	pRenderer->Destroy();
	delete pWindow;
}

void GameSystem::Initialize()
{
	//�E�B���h�E�쐬
	pWindow = new HDL_Window(WIDTH, HEIGHT, L"Rendering Engine");

	//�����_���[�쐬
	HDL_Renderer::Create(WIDTH, HEIGHT, pWindow->GetWindowHandler());
	pRenderer = HDL_Renderer::GetInstance();

	//�o�b�N�o�b�t�@�̍쐬
	pBackBuff = new HDL_BackBuffer();

	//�[�x�o�b�t�@�̍쐬
	pDSBuff = new HDL_DepthStencilBuffer();

	//Input�C���X�^���X�𐶐�
	HDL_Input::Create(pWindow->GetWindowHandler());

	//Load
	Load();
}

void GameSystem::ExcuteSystem()
{
	while (mActivation)
	{
		mActivation = pWindow->ObserveMSG();

		Input();
		Update();
		Output();
	}
}

void GameSystem::Quit()
{
	pWindow->CloseWindow();
}

void GameSystem::Input()
{
	demo->Input();
}

void GameSystem::Update()
{
	demo->Update();
}

void GameSystem::Output()
{
	//�`�揀��
	//=================================================================================
	pRenderer->EnterDrawing();
	pBackBuff->OpenBackBuffer(CLEAR_COLOR);
	pDSBuff->OpenDSBuffer();
	pRenderer->SetRenderTargets(pBackBuff->GetHeapHandle(), pDSBuff->GetHeapHandle());
	//=================================================================================
	//�`��
	//=================================================================================
	
	demo->Draw();

	//for (auto model : models)
	//{
	//	model->Draw();
	//}

	//=================================================================================
	//�`��I��
	//=================================================================================
	pBackBuff->CloseBackBuffer();
	pRenderer->ExitDrawing();
	//=================================================================================
}

void GameSystem::Load()
{
	//Model����
	demo = new Demo;
}
