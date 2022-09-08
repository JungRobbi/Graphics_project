#include "GameScene.h"
#include <iostream>


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
	p_player->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 0.0f);
	p_player->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	p_player->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 0.0f, -1.0f);

	p_player->AddComponent<Camera>();
	p_player->GetComponent<Camera>()->viewLocation = viewLocation;
	p_player->GetComponent<Camera>()->projLocation = projLocation;

	p_player->AddComponent<Collide>();
	p_player->AddComponent<Gravity>();
	p_player->AddComponent<PlayerJump>();


	// object
	if (num_scene == 1) {
		{
			CreateSkyBox(index_list, tex, vao);
		}
		{
			auto star = CreateStar(index_list,tex,vao);

			star->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 5.5f, 0.0f);
			star->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 0.3f, 1.0f);
			star->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);
			star->GetComponent<Transform3D>()->roll = 90.0f;
		}
		{
			auto box = CreateBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 5.5f, 2.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
			box->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}
	else if (num_scene == 2) {

	}
}

GameObject* GameScene::CreateBox(int* index_list, GLuint* tex, GLuint* vao) // Box 자동 생성
{
	auto box = CreateEmpty();

	box->AddComponent<Transform3D>();
	box->AddComponent<Collide>();
	box->GetComponent<Collide>()->BoundBox = BoundBox[Cube];

	//std::cout << std::endl << std::endl << BoundBox[Cube].maxX << " " << BoundBox[Cube].maxY << std::endl;
	//std::cout << BoundBox[Cube].minX << " " << BoundBox[Cube].minY << std::endl;

	box->AddComponent<Gravity>();

	// render 부분
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() 첫 번째
	box->VAO = vao[0]; // 사각형 메쉬
	box->texture = tex[1]; // 1번 텍스쳐

	return box;
}

GameObject* GameScene::CreateStar(int* index_list, GLuint* tex, GLuint* vao) // Star 자동 생성
{
	auto star = CreateEmpty();

	star->AddComponent<Transform3D>();
	star->AddComponent<Collide>();
	star->GetComponent<Collide>()->BoundBox = BoundBox[Star];

	//std::cout << std::endl << std::endl << BoundBox[Star].maxX << " " << BoundBox[Star].maxY << std::endl;
	//std::cout << BoundBox[Star].minX << " " << BoundBox[Star].minY << std::endl;

	star->AddComponent<Gravity>();

	// render 부분
	star->modelLocation = modelLocation;
	star->num_index = index_list[1]; // load() 첫 번째
	star->VAO = vao[1]; // 사각형 메쉬
	star->texture = tex[1]; // 1번 텍스쳐

	return star;
}

void GameScene::update()
{
	Scene::update();

	auto player_tran = p_player->GetComponent<Transform3D>();
	auto player_camera = p_player->GetComponent<Camera>();

	player_tran->direction.x = cos(glm::radians(player_camera->fpsup)) * cos(glm::radians(player_camera->fpsy)) + player_tran->position.x;
	player_tran->direction.y = sin(glm::radians(player_camera->fpsup)) + player_tran->position.y;
	player_tran->direction.z = cos(glm::radians(player_camera->fpsup)) * sin(glm::radians(player_camera->fpsy)) + player_tran->position.z;

}

void GameScene::CreateSkyBox(int* index_list, GLuint* tex, GLuint* vao) // SkyBox 생성
{
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 20.1f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;
		plane->GetComponent<Transform3D>()->roll = 90.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[8]; // top
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, -20.05f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[9]; // left
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(20.05f, 0.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[10]; // front
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 20.05f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = 180.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[11]; // right
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(-20.05f, 0.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = 90.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[12]; // back
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, -20.1f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;
		plane->GetComponent<Transform3D>()->roll = -90.0f;


		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[13]; // bottom
	}
}

void GameScene::render()
{
	Scene::render();
}