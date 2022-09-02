#pragma once
#include "Scene.h"

#define MS_PER_UPDATE (1'000'000 / 60) // microsec

class Framework
{
protected:
	std::list<Scene*> scenes;

	virtual void render() {}
public:

	virtual void run();

	template<typename T>
	T* AddScene()
	{
		auto scene = new T;
		scenes.push_back(scene);
		return scene;
	}
};
