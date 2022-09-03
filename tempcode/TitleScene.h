#pragma once
#include "stdafx.h"
#include "Scene.h"
#include "RenderComponent.h"
class TitleScene :
    public Scene
{

public:
	TitleScene();
	virtual ~TitleScene()
	{
		for (auto object : gameObjects)
			delete object;
		gameObjects.clear();
	}
};

