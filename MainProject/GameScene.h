#pragma once
#include "stdafx.h"
#include "Scene.h"
class GameScene :
    public Scene
{

public:
	GameScene();
	virtual ~GameScene()
	{
		for (auto object : gameObjects)
			delete object;
		gameObjects.clear();
	}
};

