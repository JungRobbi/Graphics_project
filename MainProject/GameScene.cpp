#include "GameScene.h"


GameScene::GameScene() : Scene()
{

}

GameScene::GameScene(int num_scene) : Scene()
{

}

GameScene::GameScene(int num_scene, int* index_list, GLuint* tex, GLuint* vao, GLuint* program) : Scene()
{
	n_scene = num_scene;
	modelLocation = glGetUniformLocation(program[0], "model");
	viewLocation = glGetUniformLocation(program[0], "view");
	projLocation = glGetUniformLocation(program[0], "projection");

	// player
	p_player = CreateEmpty();
	p_player->AddComponent<Transform3D>();
	p_player->AddComponent<Camera>();
	p_player->GetComponent<Camera>()->viewLocation = viewLocation;
	p_player->GetComponent<Camera>()->projLocation = projLocation;


	// object
	if (num_scene == 1) {
		{
			auto box = CreateBox(index_list, tex[1], vao); // 1�� �ؽ���
			box->GetComponent<Transform3D>()->position = glm::vec3(10.0f, 0.0f, 0.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
		}
	}
	else if (num_scene == 2) {

	}
}

GameObject* GameScene::CreateBox(int* index_list, GLuint tex, GLuint* vao) // Box �ڵ� ����
{
	auto box = CreateEmpty();

	box->AddComponent<Transform3D>();

	// render �κ�
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() ù ��°
	box->VAO = vao[0]; // �簢�� �޽�
	box->texture = tex; // 1�� �ؽ���

	return box;
}

void GameScene::update()
{
	Scene::update();
}

void GameScene::render()
{
	Scene::render();
}