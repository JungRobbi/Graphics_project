#include "GameObject.h"

GameObject::GameObject()
{
	Scene::scene->creationQueue.push(this);
}