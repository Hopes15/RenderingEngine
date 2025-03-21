#include "Model.h"
#include "GameSystem.h"

Model::Model(GameSystem* game) :
	pGame(game)
{
	pGame->AddModel(this);
}

Model::~Model()
{
	pGame->RemoveModel(this);
}

void Model::Update(float deltaTime)
{
}

void Model::Draw()
{
}
