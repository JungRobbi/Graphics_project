#include "GameScene.h"


GameScene::GameScene() : Scene()
{

}

GameScene::GameScene(int num_scene) : Scene()
{

}

GameScene::GameScene(int num_scene, int* index_list, GLuint* tex, GLuint* vao, GLuint* program) : Scene()
{
	p_s_program = program;
	//modelLocation = glGetUniformLocation(p_s_program[0], "model");

	if (num_scene == 1) {
		auto box = CreateBox(index_list, tex, vao);
	}
	else if (num_scene == 2) {

	}
}