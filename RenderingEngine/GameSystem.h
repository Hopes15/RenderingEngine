#pragma once
#include <vector>

class GameSystem
{
public:
	GameSystem();

	~GameSystem();

	void Initialize();

	void ExcuteSystem();

	void Quit();

private:
	void Input();

	void Update();

	void Output();

	void Load();

private:
	const unsigned int WIDTH;
	const unsigned int HEIGHT;
	const float		   CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float		   FIXED_DELTA_TIME;

	class HDL_Window*			  pWindow	= nullptr;
	class HDL_Renderer*			  pRenderer = nullptr;
	class HDL_BackBuffer*		  pBackBuff = nullptr;
	class HDL_DepthStencilBuffer* pDSBuff	= nullptr;
	class HDL_Input*			  pInput	= nullptr;

	std::vector<class Model*> models;

	//FILE
	const char* fileName = "";
	const char* textFile = "";

	//Models
	class Alicia*     alicia = nullptr;
	class Volkswagen* wagen  = nullptr;
};

