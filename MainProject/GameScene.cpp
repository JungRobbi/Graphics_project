#include "GameScene.h"
#include <iostream>
#include "CannonShot.h"



GameScene::GameScene() : Scene()
{

}

GameScene::GameScene(int num_scene) : Scene()
{

}

GameScene::GameScene(int num_scene, int* index_list, GLuint* tex, GLuint* vao, GLuint* program) 
	: Scene(num_scene, index_list, tex, vao, program)
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
	p_player->GetComponent<Collide>()->BoundBox = BoundBox[Cube] * 0.25;
	p_player->GetComponent<Collide>()->sub_BoundBox = p_player->GetComponent<Collide>()->BoundBox * 0.75;
	p_player->GetComponent<Collide>()->sub_BoundBox.minY = p_player->GetComponent<Collide>()->BoundBox.minY * 3;
	p_player->GetComponent<Collide>()->BoundBox.minY *= 2;

	p_player->AddComponent<Gravity>();
	p_player->AddComponent<PlayerJump>();

	
	// object
	if (num_scene == 1) {
		{
			CreateSkyBox(index_list, tex, vao);
		}
		{
			//auto temp = CreateCannon(index_list, tex, vao);
		}
		{
		/*	auto star = CreateStar(index_list,tex,vao);

			star->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 5.5f, 0.0f);
			star->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 0.3f, 1.0f);
			star->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);
			star->GetComponent<Transform3D>()->roll = 90.0f;*/
		}
		{
			/*auto box = CreateBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 5.5f, 2.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
			box->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);*/
		}
		{
			/*auto axe = CreateItem_Pickaxe(index_list, tex, vao);
			axe->GetComponent<Transform3D>()->position = glm::vec3(1.0f, 0.0f, 0.0f);
			axe->GetComponent<Transform3D>()->scale = glm::vec3(0.1f, 0.1f, 0.05f);
			axe->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);*/

		}
		{
			/*auto shoes = CreateItem_Shoes(index_list, tex, vao);
			shoes->GetComponent<Transform3D>()->roll = -90.0f;
			shoes->GetComponent<Transform3D>()->position = glm::vec3(1.0f, 0.0f, 0.2f);
			shoes->GetComponent<Transform3D>()->scale = glm::vec3(0.1f, 0.1f, 0.05f);
			shoes->GetComponent<Transform3D>()->direction = glm::vec3(0.0f, 1.0f, 0.0f);*/

		}
	}
	else if (num_scene == 2) {
		{
			CreateSkyBox(index_list, tex, vao);
		}
		{
			auto grass = CreateGrass(index_list, tex, vao);
			grass->GetComponent<Transform3D>()->position.y = -0.5;
		}
		{
			auto object = CreateItem_Pickaxe(index_list, tex, vao);
			object->GetComponent<Transform3D>()->position = glm::vec3(3.0f, 0.0f, 0.0f);
		}
		{
			//auto object = CreateItem_Shoes(index_list, tex, vao);
			//object->GetComponent<Transform3D>()->position = glm::vec3(1.0f, 0.0f, 0.0f);
			//object->GetComponent<Transform3D>()->scale = glm::vec3(3.0f, 0.1f, 3.0f);
		}
		{
			auto object = CreateCannon(index_list, tex, vao);
			object->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 3.0f);
		}
		{
			//auto object = CreateBook(index_list, tex, vao);
			//object->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 2.0f);
		}
		{
			//auto object = CreateSpike(index_list, tex, vao);
			//object->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 3.0f);
		}
	}
}

GameObject* GameScene::CreateBox(int* index_list, GLuint* tex, GLuint* vao) // Box �ڵ� ����
{
	auto box = CreateEmpty();

	box->AddComponent<Transform3D>();
	box->AddComponent<Gravity>();
	box->AddComponent<Collide>();
	box->GetComponent<Collide>()->BoundBox = BoundBox[Cube];
	box->GetComponent<Collide>()->BoundBox.pos = box->GetComponent<Transform3D>()->position;

	// render �κ�
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() ù ��°
	box->VAO = vao[0]; // �簢�� �޽�
	box->texture = tex[4]; // 1�� �ؽ���

	return box;
}

GameObject* GameScene::CreateStar(int* index_list, GLuint* tex, GLuint* vao) // Star �ڵ� ����
{
	auto star = CreateEmpty();

	star->AddComponent<Transform3D>();
	star->AddComponent<Collide>();
	star->GetComponent<Collide>()->BoundBox = BoundBox[Star];
	star->GetComponent<Collide>()->BoundBox.pos = star->GetComponent<Transform3D>()->position;

	//std::cout << std::endl << std::endl << BoundBox[Star].maxX << " " << BoundBox[Star].maxY << std::endl;
	//std::cout << BoundBox[Star].minX << " " << BoundBox[Star].minY << std::endl;

	// render �κ�
	star->modelLocation = modelLocation;
	star->num_index = index_list[1]; // load() ù ��°
	star->VAO = vao[1]; // �簢�� �޽�
	star->texture = tex[1]; // 1�� �ؽ���

	return star;
}

GameObject* GameScene::CreateItem_Pickaxe(int* index_list, GLuint* tex, GLuint* vao)
{
	auto axe = CreateEmpty();

	axe->AddComponent<Transform3D>();
	axe->AddComponent<Collide>();
	axe->GetComponent<Collide>()->BoundBox = BoundBox[Pickaxe];
	axe->GetComponent<Collide>()->BoundBox.pos = axe->GetComponent<Transform3D>()->position;
	//axe->AddComponent<ItemRotate>();

	// render �κ�
	axe->modelLocation = modelLocation;
	axe->num_index = index_list[Pickaxe]; // load()
	axe->VAO = vao[Pickaxe]; // �޽�
	axe->texture = tex[2]; // �ؽ���

	return axe;
}

GameObject* GameScene::CreateItem_Shoes(int* index_list, GLuint* tex, GLuint* vao)
{
	auto shoes = CreateEmpty();

	shoes->AddComponent<Transform3D>();
	shoes->AddComponent<Collide>();
	shoes->GetComponent<Collide>()->BoundBox = BoundBox[Shoes];
	shoes->GetComponent<Collide>()->BoundBox.pos = shoes->GetComponent<Transform3D>()->position;
	shoes->AddComponent<ItemRotate>();

	// render �κ�
	shoes->modelLocation = modelLocation;
	shoes->num_index = index_list[Shoes]; // load()
	shoes->VAO = vao[Shoes]; // �޽�
	shoes->texture = tex[2]; // �ؽ���

	return shoes;
}

GameObject* GameScene::CreateCannon(int* index_list, GLuint* tex, GLuint* vao)
{
	auto cannon = CreateEmpty();

	cannon->AddComponent<Transform3D>();
	cannon->AddComponent<CannonShot>();
	cannon->AddComponent<Collide>();
	cannon->GetComponent<Collide>()->BoundBox = BoundBox[Cannon];
	cannon->GetComponent<Collide>()->BoundBox.pos = cannon->GetComponent<Transform3D>()->position;

	// render �κ�
	cannon->modelLocation = modelLocation;
	cannon->num_index = index_list[Cannon]; //
	cannon->VAO = vao[Cannon]; //
	cannon->texture = tex[1]; // 

	return cannon;
}

GameObject* GameScene::CreateGrass(int* index_list, GLuint* tex, GLuint* vao)
{
	auto grass = CreateEmpty();

	grass->AddComponent<Transform3D>();
	grass->GetComponent<Transform3D>()->roll = -90.0f;
	grass->GetComponent<Transform3D>()->scale = glm::vec3(5.0f, 5.0f, 0.5f);

	// render �κ�
	grass->modelLocation = modelLocation;
	grass->num_index = index_list[Grass]; //
	grass->VAO = vao[Grass]; //
	grass->texture = tex[5]; // 

	return grass;
}

GameObject* GameScene::CreateBook(int* index_list, GLuint* tex, GLuint* vao)
{
	auto book = CreateEmpty();

	book->AddComponent<Transform3D>();
	book->GetComponent<Transform3D>()->scale = glm::vec3(5.0f, 5.0f, 0.5f);
	book->AddComponent<Collide>();
	book->GetComponent<Collide>()->BoundBox = BoundBox[Book];
	book->GetComponent<Collide>()->BoundBox.pos = book->GetComponent<Transform3D>()->position;


	// render �κ�
	book->modelLocation = modelLocation;
	book->num_index = index_list[Book]; //
	book->VAO = vao[Book]; //
	book->texture = tex[2]; // 

	return book;
}

GameObject* GameScene::CreateSpike(int* index_list, GLuint* tex, GLuint* vao)
{
	auto spike = CreateEmpty();

	spike->AddComponent<Transform3D>();
	spike->GetComponent<Transform3D>()->scale = glm::vec3(5.0f, 5.0f, 0.5f);
	spike->AddComponent<Collide>();
	spike->GetComponent<Collide>()->BoundBox = BoundBox[Spike];
	spike->GetComponent<Collide>()->BoundBox.pos = spike->GetComponent<Transform3D>()->position;

	// render �κ�
	spike->modelLocation = modelLocation;
	spike->num_index = index_list[Spike]; //
	spike->VAO = vao[Spike]; //
	spike->texture = tex[2]; // 

	return spike;
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

void GameScene::CreateSkyBox(int* index_list, GLuint* tex, GLuint* vao) // SkyBox ����
{
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 20.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;
		plane->GetComponent<Transform3D>()->roll = 90.0f;

		// render �κ�
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() ù ��°
		plane->VAO = vao[2]; // �簢�� �޽�
		plane->texture = tex[8]; // top
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, -19.88f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);

		// render �κ�
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() ù ��°
		plane->VAO = vao[2]; // �簢�� �޽�
		plane->texture = tex[9]; // left
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(19.88f, 0.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;

		// render �κ�
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() ù ��°
		plane->VAO = vao[2]; // �簢�� �޽�
		plane->texture = tex[10]; // front
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 19.88f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = 180.0f;

		// render �κ�
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() ù ��°
		plane->VAO = vao[2]; // �簢�� �޽�
		plane->texture = tex[11]; // right
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(-19.88f, 0.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = 90.0f;

		// render �κ�
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() ù ��°
		plane->VAO = vao[2]; // �簢�� �޽�
		plane->texture = tex[12]; // back
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, -20.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;
		plane->GetComponent<Transform3D>()->roll = -90.0f;


		// render �κ�
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() ù ��°
		plane->VAO = vao[2]; // �簢�� �޽�
		plane->texture = tex[13]; // bottom
	}
}

void GameScene::render()
{
	Scene::render();
}

