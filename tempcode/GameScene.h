#pragma once
#include "stdafx.h"
#include "Scene.h"

#include "Transform3D.h"
#include "RenderComponent.h"

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

	void CreatePlayer()
	{
		auto player = CreateEmpty();
	}
	
	GameObject* CreateBox()
	{
		auto box = CreateEmpty();

		box->AddComponent<Transform3D>();
		box->AddComponent<RenderComponent>();
		box->GetComponent<RenderComponent>()->SetNumImage(4);
		//box->GetComponent<RenderComponent>()->SetNumIndex(loadObj_normalize_center_3f("Resource/cube.obj"));

		return box;
	}
};

