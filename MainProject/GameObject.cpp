#include "GameObject.h"

GameObject::GameObject()
{
	Scene::scene->creationQueue.push(this);
}

void GameObject::render()
{

}