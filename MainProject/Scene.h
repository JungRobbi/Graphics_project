#pragma once

#include <list>
#include <queue>
#include <deque>

#include "stdafx.h"
#include "GameObject.h"

class GameObject;

class Scene
{
	std::queue<GameObject*> creationQueue;
	std::deque<GameObject*> deletionQueue;

public:
	static Scene* scene;

	GameObject* p_player;
	int n_scene;
	int* p_index_list;
	GLuint* p_tex;
	GLuint* p_vao;
	GLuint* p_s_program;

public:
	GameObject* CreateEmpty();
	

public:
	std::list<GameObject*> gameObjects;
	Scene();
	Scene(int n_scene, int* p_index_list, GLuint* p_tex, GLuint* p_vao, GLuint* p_s_program);
	virtual ~Scene()
	{
		for (auto object : gameObjects)
			delete object;
		gameObjects.clear();
	}
	virtual void update();
	virtual void render();

	void PushDelete(GameObject* gameObject)
	{
		if (std::find(deletionQueue.begin(), deletionQueue.end(), gameObject) == deletionQueue.end())
			deletionQueue.push_back(gameObject);
	}

	virtual GameObject* CreateBox(int* index_list, GLuint* tex, GLuint* vao) { return nullptr; }
	virtual GameObject* CreateStar(int* index_list, GLuint* tex, GLuint* vao) { return nullptr; }
	virtual GameObject* CreateAirBox(int* index_list, GLuint* tex, GLuint* vao) { return nullptr; }
	virtual GameObject* CreateAirHardBox(int* index_list, GLuint* tex, GLuint* vao) { return nullptr; }
	virtual void CreateSkyBox(int* index_list, GLuint* tex, GLuint* vao) {};

	friend GameObject;
};