#include "Scene.h"

#include <algorithm>

Scene* Scene::scene{ nullptr };

Scene::Scene()
{
	scene = this;
}

Scene::Scene(int n_scene, int* p_index_list, GLuint* p_tex, GLuint* p_vao, GLuint* p_s_program)
	: n_scene{ n_scene }, p_index_list{ p_index_list }, p_tex{ p_tex }, p_vao{ p_vao }, p_s_program{p_s_program}
{
	scene = this;
}

GameObject* Scene::CreateEmpty()
{
	return new GameObject();
}

void Scene::update()
{
	while (!creationQueue.empty())
	{
		auto gameObject = creationQueue.front();
		gameObject->start();
		gameObjects.push_back(gameObject);
		creationQueue.pop();
	}

	for (auto gameObject : gameObjects)
		gameObject->update();

	auto t = deletionQueue;
	while (!deletionQueue.empty())
	{
		auto gameObject = deletionQueue.front();
		gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), gameObject));
		deletionQueue.pop_front();

		delete gameObject;
	}
}

void Scene::render()
{
	for (auto object : Scene::scene->gameObjects) {
		object->render();
	}
}