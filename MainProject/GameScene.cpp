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

	if (num_scene == 1) {
		{
			auto box = CreateEmpty();

			box->AddComponent<Transform3D>();

			// render 부분
			box->modelLocation = modelLocation;
			box->num_index = index_list[1]; // load() 첫 번째
			box->VAO = vao[1]; // 사각형 메쉬
			box->texture = tex[1]; // 1번 텍스쳐

			box->GetComponent<Transform3D>()->rotate = glm::vec3(0.0f, 90.0f, 0.0f);
			box->GetComponent<Transform3D>()->position = glm::vec3(10.0f, 0.0f, 0.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 0.0f, 1.0f);
		}
	}
	else if (num_scene == 2) {

	}
}

GameObject* GameScene::CreateBox(int* index_list, GLuint tex, GLuint* vao) // Box 자동 생성
{
	auto box = CreateEmpty();

	box->AddComponent<Transform3D>();

	// render 부분
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() 첫 번째
	box->VAO = vao[0]; // 사각형 메쉬
	box->texture = tex; // 1번 텍스쳐

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