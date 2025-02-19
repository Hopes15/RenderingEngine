#include "GameSystem.h"
#include "HDL_Renderer.h"
#include "HDL_Window.h"
#include "HDL_BackBuffer.h"
#include "HDL_DepthStencilBuffer.h"
#include "GameObject.h"

GameSystem::GameSystem() :
	WIDTH(1920),
	HEIGHT(1080),
	FIXED_DELTA_TIME(0.02f),
	mActivation(true)
{
}

GameSystem::~GameSystem()
{
	delete pGameObject;
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

	//GameObject����
	pGameObject = new GameObject();
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
}

void GameSystem::Update()
{
	pGameObject->Update(FIXED_DELTA_TIME);
}

void GameSystem::Output()
{
	pRenderer->EnterDrawing();
	pBackBuff->OpenBackBuffer(CLEAR_COLOR);
	pDSBuff->OpenDSBuffer();
	pRenderer->SetRenderTargets(pBackBuff->GetHeapHandle(), pDSBuff->GetHeapHandle());

	pGameObject->Draw();

	pBackBuff->CloseBackBuffer();
	pRenderer->ExitDrawing();
}
