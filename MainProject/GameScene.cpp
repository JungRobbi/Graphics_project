#include "GameScene.h"


GameScene::GameScene() : Scene()
{

}

GameScene::GameScene(int num_scene) : Scene()
{

}

GameScene::GameScene(int num_scene, int index, GLint* tex, GLint* vao) : Scene()
{
	if (num_scene == 1) {
		auto box = CreateBox(index, tex, vao);
	}
	else if (num_scene == 2) {

	}


}