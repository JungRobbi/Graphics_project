#pragma once

#include <list>
#include <queue>
#include <deque>
#include "GameObject.h"

class Scene
{
	std::queue<GameObject*> creationQueue;
	std::deque<GameObject*> deletionQueue;

	std::list<GameObject*> gameObjects;

public:
	static Scene* scene;

protected:
	GameObject* CreateEmpty();

public:
	Scene();
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

	friend GameObject;
};