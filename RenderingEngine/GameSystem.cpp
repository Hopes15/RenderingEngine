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
	//ウィンドウ作成
	pWindow = new HDL_Window(WIDTH, HEIGHT, L"Rendering Engine");

	//レンダラー作成
	HDL_Renderer::Create(WIDTH, HEIGHT, pWindow->GetWindowHandler());
	pRenderer = HDL_Renderer::GetInstance();

	//バックバッファの作成
	pBackBuff = new HDL_BackBuffer();

	//深度バッファの作成
	pDSBuff = new HDL_DepthStencilBuffer();

	//GameObject生成
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
