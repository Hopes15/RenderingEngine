#include "GameSystem.h"
#include "HDL_Renderer.h"
#include "HDL_Window.h"
#include "HDL_BackBuffer.h"
#include "GameObject.h"

GameSystem::GameSystem() :
	WIDTH(1920),
	HEIGHT(1080),
	mActivation(true)
{
}

GameSystem::~GameSystem()
{
	delete pGameObject;
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
	pGameObject->Update();
}

void GameSystem::Output()
{
	pRenderer->EnterDrawing();
	pBackBuff->OpenBackBuffer(CLEAR_COLOR);

	pGameObject->Draw();

	pBackBuff->CloseBackBuffer();
	pRenderer->ExitDrawing();
}
