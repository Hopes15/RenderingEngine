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
	//�E�B���h�E�쐬
	pWindow = new HDL_Window(WIDTH, HEIGHT, L"No Name");

	//�����_���[�쐬
	HDL_Renderer::Create(WIDTH, HEIGHT, pWindow->GetWindowHandler());
	pRenderer = HDL_Renderer::GetInstance();

	//�o�b�N�o�b�t�@�̍쐬
	pBackBuff = new HDL_BackBuffer();

	//�[�x�o�b�t�@�̍쐬
	pDSBuff = new HDL_DepthStencilBuffer();

	//Input�C���X�^���X�𐶐�
	HDL_Input::Create(pWindow->GetWindowHandler());

#if 0
	//�R���o�[�g���鎞����
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
	//�`�揀��
	//=================================================================================
	pRenderer->EnterDrawing();
	pBackBuff->OpenBackBuffer(CLEAR_COLOR);
	pDSBuff->OpenDSBuffer();
	pRenderer->SetRenderTargets(pBackBuff->GetHeapHandle(), pDSBuff->GetHeapHandle());
	//=================================================================================
	//�`��
	//=================================================================================
	
	alicia->Draw();
	wagen->Draw();

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
	pCam = new Camera;
	pCam->Init();
	alicia = new Alicia(pCam);
	wagen = new Volkswagen(pCam);
}
