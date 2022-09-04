#pragma once
#include "stdafx.h"
#include "Scene.h"

#include "Transform3D.h"

class GameScene : public Scene
{

public:
	unsigned int modelLocation;

	GLuint* p_s_program;

public:
	GameScene();
	GameScene(int);
	GameScene(int num_scene, int* index_list, GLuint* tex, GLuint* vao, GLuint* program);
	virtual ~GameScene()
	{
		for (auto object : gameObjects)
			delete object;
		gameObjects.clear();
	}

	virtual void update();
	virtual void render();

	
	GameObject* CreateBox(int* index_list, GLuint tex, GLuint* vao);

};

