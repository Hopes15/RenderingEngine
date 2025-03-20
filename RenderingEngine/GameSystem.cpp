#include "GameSystem.h"
#include "HDL_Renderer.h"
#include "HDL_Window.h"
#include "HDL_BackBuffer.h"
#include "HDL_DepthStencilBuffer.h"
#include "Model.h"
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
	UnLoad();
	pWindow->CloseWindow();
}

void GameSystem::Input()
{
}

void GameSystem::Update()
{
	demo->Update();

	//for (auto model : models)
	//{
	//	model->Update(0.02f);
	//}
}

void GameSystem::Output()
{
	//描画準備
	//=================================================================================
	pRenderer->EnterDrawing();
	pBackBuff->OpenBackBuffer(CLEAR_COLOR);
	pDSBuff->OpenDSBuffer();
	pRenderer->SetRenderTargets(pBackBuff->GetHeapHandle(), pDSBuff->GetHeapHandle());
	//=================================================================================
	//描画
	//=================================================================================
	
	demo->Draw();

	//for (auto model : models)
	//{
	//	model->Draw();
	//}

	//=================================================================================
	//描画終了
	//=================================================================================
	pBackBuff->CloseBackBuffer();
	pRenderer->ExitDrawing();
	//=================================================================================
}

void GameSystem::Load()
{
	//Model生成
	demo = new Demo;
}

void GameSystem::UnLoad()
{
	//while (!models.empty())
	//{
	//	delete models.back();
	//}
}

void GameSystem::AddModel(Model* model)
{
	models.emplace_back(model);
}

void GameSystem::RemoveModel(Model* model)
{
	auto iter = std::find(models.begin(), models.end(), model);
	models.erase(iter);
}
