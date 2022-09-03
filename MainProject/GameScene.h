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

	
	GameObject* CreateBox(int* index_list, GLuint* tex, GLuint* vao)
	{
		auto box = CreateEmpty();

		box->AddComponent<Transform3D>();
		box->num_index = index_list[0];
		box->VAO = vao[0];
		box->s_program = p_s_program[0];
		box->texture = tex[1];

		return box;
	}

};

