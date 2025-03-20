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

	void AddModel(class Model* model);

	void RemoveModel(class Model* model);

private:
	void Input();

	void Update();

	void Output();

	void Load();

	void UnLoad();

private:
	const unsigned int WIDTH;
	const unsigned int HEIGHT;
	const float		   CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float		   FIXED_DELTA_TIME;

	class HDL_Window*			  pWindow	= nullptr;
	class HDL_Renderer*			  pRenderer = nullptr;
	class HDL_BackBuffer*		  pBackBuff = nullptr;
	class HDL_DepthStencilBuffer* pDSBuff   = nullptr;

	bool mActivation;

	std::vector<class Model*> models;

	class Demo* demo = nullptr;
};

