#pragma once

class Model
{
public:
	Model(class GameSystem* game);

	virtual ~Model();

	virtual void Update(float deltaTime);

	virtual void Draw();

protected:
	class GameSystem* pGame = nullptr;
};

