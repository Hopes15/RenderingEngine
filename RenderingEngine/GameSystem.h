#pragma once

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

private:
	const unsigned int WIDTH;
	const unsigned int HEIGHT;
	const float		   CLEAR_COLOR[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	class HDL_Window*     pWindow	= nullptr;
	class HDL_Renderer*   pRenderer = nullptr;
	class HDL_BackBuffer* pBackBuff = nullptr;

	class GameObject*	  pGameObject = nullptr;

	bool mActivation;
};

