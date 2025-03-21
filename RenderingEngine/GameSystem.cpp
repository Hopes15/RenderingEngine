#include "GameSystem.h"
#include "HDL_Renderer.h"
#include "HDL_Window.h"
#include "HDL_BackBuffer.h"
#include "HDL_DepthStencilBuffer.h"
#include "HDL_Input.h"
#include "Alicia.h"
#include "Converter.h"
#include "Volkswagen.h"

GameSystem::GameSystem() :
	WIDTH(1920),
	HEIGHT(1080),
	FIXED_DELTA_TIME(0.02f),
	mActivation(true)
{
}

GameSystem::~GameSystem()
{
	delete wagen;
	delete alicia;
	pInput->Destroy();
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

	//Inputインスタンスを生成
	HDL_Input::Create(pWindow->GetWindowHandler());

#if 0
	//コンバートする時だけ
	Converter converter;
	converter.ConvertFBX(fileName, textFile, scale, scale, scale, 0, 1, 2);
#endif

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
	alicia->Input();
}

void GameSystem::Update()
{
	alicia->Update();
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
	
	alicia->Draw();
	wagen->Draw();

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
	alicia = new Alicia;
	wagen = new Volkswagen;
}
