#include "GameScene.h"


GameScene::GameScene() : Scene()
{

}

GameScene::GameScene(int num_scene) : Scene()
{

}

GameScene::GameScene(int num_scene, int* index_list, GLuint* tex, GLuint* vao, GLuint* program) : Scene()
{
	modelLocation = glGetUniformLocation(program[0], "model");

	if (num_scene == 1) {
		auto box = CreateBox(index_list, tex, vao);
	}
	else if (num_scene == 2) {

	}
}

GameObject* GameScene::CreateBox(int* index_list, GLuint* tex, GLuint* vao) // Box �ڵ� ����
{
	auto box = CreateEmpty();

	box->AddComponent<Transform3D>();

	// render �κ�
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() ù ��°
	box->VAO = vao[0]; // �簢�� �޽�
	box->texture = tex[1]; // 1�� �ؽ���

	return box;
}