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
	GameScene(int);
	GameScene(int, int, GLint*, GLint*);
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
	
	GameObject* CreateBox(int index, GLint* tex, GLint* vao)
	{
		auto box = CreateEmpty();

		box->AddComponent<Transform3D>();
		//box->AddComponent<RenderComponent>();
		//box->GetComponent<RenderComponent>()->SetNumImage(4);
		//box->GetComponent<RenderComponent>()->SetNumIndex(index);
		//box->GetComponent<RenderComponent>()->p_texture = tex;
		//box->GetComponent<RenderComponent>()->p_VAO = vao;

		return box;
	}
};

