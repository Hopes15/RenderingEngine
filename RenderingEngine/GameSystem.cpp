#include "GameSystem.h"
#include "HDL_Renderer.h"
#include "HDL_Window.h"
#include "HDL_BackBuffer.h"
#include "HDL_DepthStencilBuffer.h"
#include "HDL_Input.h"
#include "Alicia.h"
#include "Converter.h"
#include "Volkswagen.h"
#include "Camera.h"

GameSystem::GameSystem() :
	WIDTH(1920),
	HEIGHT(1080),
	FIXED_DELTA_TIME(0.02f)
{
}

GameSystem::~GameSystem()
{
	delete wagen;
	delete alicia;
	delete pCam;
	pInput->Destroy();
	delete pDSBuff;
	delete pBackBuff;
	pRenderer->Destroy();
	delete pWindow;
}

void GameSystem::Initialize()
{
	//ウィンドウ作成
	pWindow = new HDL_Window(WIDTH, HEIGHT, L"No Name");

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
	while (pWindow->ObserveMSG())
	{
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
	//pCam->Input();
}

void GameSystem::Update()
{
	alicia->Update();
	//pCam->Update();
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
	pCam = new Camera;
	pCam->Init();
	alicia = new Alicia(pCam);
	wagen = new Volkswagen(pCam);
}
