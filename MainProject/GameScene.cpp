#include "GameScene.h"
#include <iostream>
#include "CannonShot.h"
#include "DestroyEffect.h"
#include "LavaMove.h"



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
			auto grass = CreateGrass(index_list, tex, vao);
			grass->GetComponent<Transform3D>()->position.y = -0.5;
			grass->GetComponent<Transform3D>()->scale = glm::vec3(9.0f, 9.0f, 1.0f);
			grass->texture = tex[5];
		}

		{
			auto box = CreateAirHardBox(index_list,tex,vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(1.0f, 0.0f, 0.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto box2 = CreateAirHardBox(index_list, tex, vao);

			box2->GetComponent<Transform3D>()->position = glm::vec3(1.8f, 0.8f, 0.0f);
			box2->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto box3 = CreateAirHardBox(index_list, tex, vao);

			box3->GetComponent<Transform3D>()->position = glm::vec3(2.6f, 1.6f, 0.0f);
			box3->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto box4 = CreateAirHardBox(index_list, tex, vao);

			box4->GetComponent<Transform3D>()->position = glm::vec3(3.4f, 2.4f, 0.0f);
			box4->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto box5 = CreateAirHardBox(index_list, tex, vao);

			box5->GetComponent<Transform3D>()->position = glm::vec3(4.2f, 3.2f, 0.0f);
			box5->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto box6 = CreateAirBox(index_list, tex, vao);

			box6->GetComponent<Transform3D>()->position = glm::vec3(3.4f, 4.0f, 0.0f);
			box6->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto axe = CreateItem_Pickaxe(index_list, tex, vao);

			axe->GetComponent<Transform3D>()->position = glm::vec3(2.6f, 2.4f, 0.0f);
			axe->GetComponent<Transform3D>()->scale = glm::vec3(0.1f, 0.2f, 0.3f);

			auto box7 = CreateAirHardBox(index_list, tex, vao);

			box7->GetComponent<Transform3D>()->position = glm::vec3(5.8f, 4.8f, 0.0f);
			box7->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto box8 = CreateAirHardBox(index_list, tex, vao);

			box8->GetComponent<Transform3D>()->position = glm::vec3(6.6f, 5.6f, 0.0f);
			box8->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto box9 = CreateAirHardBox(index_list, tex, vao);

			box9->GetComponent<Transform3D>()->position = glm::vec3(7.4f, 5.6f, 0.0f);
			box9->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

			auto shoe = CreateItem_Shoes(index_list, tex, vao);

			shoe->GetComponent<Transform3D>()->position = glm::vec3(4.2f, 4.0f, 0.0f);
			shoe->GetComponent<Transform3D>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);

			auto star = CreateStar(index_list, tex, vao);

			star->GetComponent<Transform3D>()->position = glm::vec3(7.4f, 9.0f, 0.0f);
			star->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
			star->AddComponent<Transform3D>()->roll = 90.0f;

		}
	
	}
	else if (num_scene == 2) {
		p_player->GetComponent<Transform3D>()->position.y = 3.0f;
		for (int i = 0; i < 49; ++i) {
			p_player->Item_bag.push_back(Pickaxe);
		}
		{
			CreateSkyBox(index_list, tex, vao);
		}
		{
			auto hot = CreateHot(index_list, tex, vao);
			hot->GetComponent<Transform3D>()->position.y = 0.0;
			hot->GetComponent<Transform3D>()->scale = glm::vec3(15.0f, 15.0f, 1.0f);
		}
		{
			for (int i = -3; i < 4; ++i) {
				for (int j = 0; j < 7; ++j) {
					if ((i == 0 && j == 0) || (i == 0 && j == 1) || (i == 0 && j == 2) ||
						(i == 1 && j == 2) || (i == 2 && j == 2) || (i == 2 && j == 3) ||
						(i == 2 && j == 4) || (i == 1 && j == 4) || (i == 0 && j == 4) ||
						(i == -1 && j == 4) || (i == -2 && j == 4) || (i == -2 && j == 5)
						|| (i == -2 && j == 6)) {
						auto box = CreateAirHardBox(index_list, tex, vao);

						box->GetComponent<Transform3D>()->position = glm::vec3(2.0 * i, 1.0f, 2.0 * j);
						box->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
						box->texture = tex[1]; // 1번 텍스쳐
					}
					else {
						auto box = CreateAirBox(index_list, tex, vao);

						box->GetComponent<Transform3D>()->position = glm::vec3(2.0 * i, 1.0f, 2.0 * j);
						box->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
						box->texture = tex[1]; // 1번 텍스쳐
					}
				}
			}
			{
				auto box = CreateAirHardBox(index_list, tex, vao);

				box->GetComponent<Transform3D>()->position = glm::vec3(-4.0, 1.0f, 14.0);
				box->GetComponent<Transform3D>()->scale = glm::vec3(1.0f, 1.0f, 1.0f);
			}
			{
				auto star = CreateStar(index_list, tex, vao);

				star->GetComponent<Transform3D>()->position = glm::vec3(-4.0, 3.0f, 14.0);
				star->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
				star->AddComponent<Transform3D>()->roll = 90.0f;
			}
		}
		
	}
	else if (num_scene == 3) {
		{
			CreateSkyBox(index_list, tex, vao);
		}
		{
			auto axe = CreateItem_Pickaxe(index_list, tex, vao);
			axe->GetComponent<Transform3D>()->position = glm::vec3(-7.35f, 0.5f, -6.525f);
			axe->GetComponent<Transform3D>()->scale = glm::vec3(0.2f, 0.4f, 0.6f);
		}
		{
			auto star = CreateStar(index_list, tex, vao);
			star->GetComponent<Transform3D>()->position = glm::vec3(-7.3125f, 0.5f, 7.5f);
			star->GetComponent<Transform3D>()->scale = glm::vec3(0.8f, 0.8f, 0.8f);
			star->AddComponent<Transform3D>()->roll = 90.0f;
		}
		{
			auto box = CreateAirBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-5.25f, 0.4f, 7.55f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.8f, 0.8f, 0.8f);
		}
		{
			CreateSkyBox(index_list, tex, vao);
			auto grass = CreateGrass(index_list, tex, vao);
			grass->GetComponent<Transform3D>()->position.y = -0.8;
			grass->GetComponent<Transform3D>()->scale = glm::vec3(9.0f, 9.0f, 1.0f);
			grass->texture = tex[5];
		}

		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-1.0f, 0.0f, 0.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 1.2f);
			//box->texture = tex[21];
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, -1.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(1.2f, 2.0f, 0.4f);
			//box->texture = tex[21];
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 1.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(1.2f, 2.0f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(2.6f, 0.0f, 0.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 2.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, -2.8f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(2.8f, 2.0f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 2.8f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(2.8f, 2.0f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-2.6f, 0.0f, -0.6f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 2.0f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-4.5f, 0.0f, -1.2f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 3.8f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-6.4f, 0.0f, 0.2f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 5.6f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-8.6f, 0.0f, 0.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 9.0f);
		}
		//
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, -5.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(4.0f, 2.0f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, -7.6f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(8.8f, 2.0f, 0.4f);
		}

		//

		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(4.7f, 0.0f, 0.2f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 3.5f);
		}

		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(6.9f, 0.0f, 1.2f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 2.0f, 8.4f);
		}
		
		//

		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-2.6f, 0.0f, 6.2f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(7.0f, 2.0f, 0.4f);
		}

		{
			auto box = CreateAirHardBox(index_list, tex, vao);
			box->GetComponent<Transform3D>()->position = glm::vec3(-2.0f, 0.0f, 9.2f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(8.8f, 2.0f, 0.4f);
		}
	}

	else if (num_scene == 4) {
		p_player->GetComponent<Transform3D>()->position.y = 3.0f;
		{
			CreateSkyBox(index_list, tex, vao);
		}
		//auto hot = CreateGrass(index_list, tex, vao);
		//hot->GetComponent<Transform3D>()->position.y = 0.0;
		//hot->GetComponent<Transform3D>()->scale = glm::vec3(15.0f, 15.0f, 0.7f);
		for (int i = -4; i < 5; ++i) {
			for (int j = 0; j < 20; ++j) {
				if (j == 4 || j == 5||j==6 || j == 7 || j == 8 || j == 12 ||j==13|| j == 14 || j == 15 || j == 16) {
					auto spike = CreateSpike(index_list, tex, vao);

					spike->GetComponent<Transform3D>()->position = glm::vec3(0.8 * i, 1.0f, 0.8 * j);
					spike->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
					spike->texture = tex[6]; // 1번 텍스쳐
				}
				else {

					auto box = CreateAirHardBox(index_list, tex, vao);

					box->GetComponent<Transform3D>()->position = glm::vec3(0.8 * i, 1.0f, 0.8 * j);
					box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

				}

			}
		}
		{
			auto box = CreateAirBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(3.0f, 1.8f, 2.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

		}
		{
			auto box = CreateAirBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(3.0f, 1.8f, 2.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

		}
		{
			auto star = CreateStar(index_list, tex, vao);
			star->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 1.8f, 15.0f);
			star->GetComponent<Transform3D>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
			star->AddComponent<Transform3D>()->roll = 90.0f;
		}
		auto shoe = CreateItem_Shoes(index_list, tex, vao);

		shoe->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 4.0f, 9.0f);
		shoe->GetComponent<Transform3D>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);
		{
			auto axe = CreateItem_Pickaxe(index_list, tex, vao);

			axe->GetComponent<Transform3D>()->position = glm::vec3(-2.0f, 1.8f, 8.0f);
			axe->GetComponent<Transform3D>()->scale = glm::vec3(0.1f, 0.2f, 0.3f);
		}
		{
			auto box = CreateAirBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(4.0f, 1.8f, 8.0f);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

		}
		{
			auto axe = CreateItem_Pickaxe(index_list, tex, vao);

			axe->GetComponent<Transform3D>()->position = glm::vec3(-3.0f, 1.8f, 2.0f);
			axe->GetComponent<Transform3D>()->scale = glm::vec3(0.1f, 0.2f, 0.3f);
		}
		{
			auto axe = CreateItem_Pickaxe(index_list, tex, vao);

			axe->GetComponent<Transform3D>()->position = glm::vec3(-2.0f, 3.0f, 3.2);
			axe->GetComponent<Transform3D>()->scale = glm::vec3(0.1f, 0.2f, 0.3f);
		}
		{
			auto box = CreateAirBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-2.8f, 3.0f, 3.2);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);

		}

		

	}

	else if (num_scene == 5) {
		float x = 0.8f;
		float y = 0.8f;
		float z = 0.8f;
		{
			CreateSkyBox(index_list, tex, vao);
		}
		{
			auto grass = CreateGrass(index_list, tex, vao);
			grass->GetComponent<Transform3D>()->position.y = -0.5;
			grass->GetComponent<Transform3D>()->scale = glm::vec3(9.0f, 9.0f, 1.0f);
			grass->texture = tex[7];
		}
		{
			auto star = CreateStar(index_list, tex, vao);

			star->GetComponent<Transform3D>()->position = glm::vec3(x * 4, y * 7.5, z * 7);
			star->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
			star->AddComponent<Transform3D>()->roll = 90.0f;
		}
		{
			auto axe = CreateItem_Pickaxe(index_list, tex, vao);

			axe->GetComponent<Transform3D>()->position = glm::vec3(-x * 3, 0, 0);
			axe->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(x * 4, y * 4, -z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(x * 2.5, 0, z * 2.5);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(-x * 2.5, 0, z * 2.5);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(x * 2.5, 0, -z * 2.5);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(-x * 2.5, 0, -z * 2.5);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, 0, -z * 7);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(-x * 3, 0, -z * 7);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(x * 3, 0, -z * 7);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(x * 4, 0, -z * 7);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, 0, z * 7);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(-x * 3, 0, z * 7);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(x * 3, 0, z * 7);
		}
		{
			auto spike = CreateSpike(index_list, tex, vao);

			spike->GetComponent<Transform3D>()->position = glm::vec3(x * 4, 0, z * 7);
		}

		// 위
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, 0, 0);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, 0, -z);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, y, -z);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, 0, -z * 4);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, y, -z*4);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		for (int i{}; i < 3; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, y*i, -z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		for (int i{}; i < 4; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 3, y * i, -z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto shoes = CreateItem_Shoes(index_list, tex, vao);

			shoes->GetComponent<Transform3D>()->position = glm::vec3(0, y * 4, -z * 7);
			shoes->GetComponent<Transform3D>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);
		}
		for (int i{}; i < 4; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(x * 3, y * i, -z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		for (int i{}; i < 4; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(x * 4, y * i, -z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}

		// 아래
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, 0, z);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, y, z);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, 0, z * 4);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, y, z * 4);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		for (int i{}; i < 3; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 6, y * i, z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		for (int i{}; i < 4; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(-x * 3, y * i, z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		{
			auto shoes = CreateItem_Shoes(index_list, tex, vao);

			shoes->GetComponent<Transform3D>()->position = glm::vec3(0, y * 4, z * 7);
			shoes->GetComponent<Transform3D>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);
		}
		for (int i{}; i < 4; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(x * 3, y * i, z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}
		for (int i{}; i < 4; ++i) {
			auto box = CreateAirHardBox(index_list, tex, vao);

			box->GetComponent<Transform3D>()->position = glm::vec3(x * 4, y * i, z * 7);
			box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
		}

	}
	else if (num_scene == 6) {
		p_player->GetComponent<Transform3D>()->position = glm::vec3(-4.0f, 0.0f, 0.0f);
		{
			CreateSkyBox(index_list, tex, vao);
		}
		{
			auto grass = CreateGrass(index_list, tex, vao);
			grass->GetComponent<Transform3D>()->position = glm::vec3(3.0f, -0.5f, 0.0f);
			grass->GetComponent<Transform3D>()->scale = glm::vec3(10.0f, 5.0f, 0.5f);
			grass->texture = tex[7];
		}
		for (int i{}; i < 7; ++i) {
			auto cannon = CreateCannon(index_list, tex, vao);
			cannon->GetComponent<Transform3D>()->position = glm::vec3(i * 1.0f, 0.0f, -4.5f);
			cannon->GetComponent<Transform3D>()->scale = glm::vec3(0.8f, 0.8f, 0.8f);
			cannon->GetComponent<Transform3D>()->yaw = -90.0f;
			cannon->texture = tex[1];
		}
	}
}

GameObject* GameScene::CreateBox(int* index_list, GLuint* tex, GLuint* vao) // Box 자동 생성
{
	auto box = CreateEmpty();

	box->AddComponent<Transform3D>();
	box->AddComponent<DestroyEffect>();
	box->AddComponent<Gravity>();
	box->AddComponent<Collide>();
	box->GetComponent<Collide>()->BoundBox = BoundBox[Cube];
	box->GetComponent<Collide>()->BoundBox.pos = box->GetComponent<Transform3D>()->position;

	// render 부분
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() 첫 번째
	box->VAO = vao[0]; // 사각형 메쉬
	box->texture = tex[3]; // 1번 텍스쳐

	return box;
}

GameObject* GameScene::CreateAirBox(int* index_list, GLuint* tex, GLuint* vao) // Box 자동 생성
{
	auto box = CreateEmpty();


	box->AddComponent<Transform3D>();
	box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
	box->AddComponent<DestroyEffect>();
	box->AddComponent<Collide>();
	box->GetComponent<Collide>()->BoundBox = BoundBox[Cube];
	box->GetComponent<Collide>()->BoundBox.pos = box->GetComponent<Transform3D>()->position;

	// render 부분
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() 첫 번째
	box->VAO = vao[0]; // 사각형 메쉬
	box->texture = tex[3]; // 1번 텍스쳐

	return box;
}

GameObject* GameScene::CreateAirHardBox(int* index_list, GLuint* tex, GLuint* vao) // Box 자동 생성
{
	auto box = CreateEmpty();


	box->AddComponent<Transform3D>();
	box->GetComponent<Transform3D>()->scale = glm::vec3(0.4f, 0.4f, 0.4f);
	box->AddComponent<Collide>();
	box->GetComponent<Collide>()->BoundBox = BoundBox[Cube];
	box->GetComponent<Collide>()->BoundBox.pos = box->GetComponent<Transform3D>()->position;

	// render 부분
	box->modelLocation = modelLocation;
	box->num_index = index_list[0]; // load() 첫 번째
	box->VAO = vao[0]; // 사각형 메쉬
	box->texture = tex[4]; // 1번 텍스쳐

	return box;
}

GameObject* GameScene::CreateStar(int* index_list, GLuint* tex, GLuint* vao) // Star 자동 생성
{
	auto star = CreateEmpty();

	star->AddComponent<Transform3D>();
	star->AddComponent<Collide>();
	star->GetComponent<Collide>()->BoundBox = BoundBox[Star];
	star->GetComponent<Collide>()->BoundBox.pos = star->GetComponent<Transform3D>()->position;
	star->AddComponent<Transform3D>()->pitch = 90.0f;
	star->AddComponent<ItemRotate>();

	//std::cout << std::endl << std::endl << BoundBox[Star].maxX << " " << BoundBox[Star].maxY << std::endl;
	//std::cout << BoundBox[Star].minX << " " << BoundBox[Star].minY << std::endl;

	// render 부분
	star->modelLocation = modelLocation;
	star->num_index = index_list[1]; // load() 첫 번째
	star->VAO = vao[1]; // 사각형 메쉬
	star->texture = tex[2]; // 1번 텍스쳐

	return star;
}

GameObject* GameScene::CreateItem_Pickaxe(int* index_list, GLuint* tex, GLuint* vao)
{
	auto axe = CreateEmpty();

	axe->AddComponent<Transform3D>();
	axe->GetComponent<Transform3D>()->scale = glm::vec3(0.05f, 0.05f, 0.05f);
	axe->AddComponent<Collide>();
	axe->GetComponent<Collide>()->BoundBox = BoundBox[Pickaxe];
	axe->GetComponent<Collide>()->BoundBox.pos = axe->GetComponent<Transform3D>()->position;
	axe->AddComponent<ItemRotate>();

	// render 부분
	axe->modelLocation = modelLocation;
	axe->num_index = index_list[Pickaxe]; // load()
	axe->VAO = vao[Pickaxe]; // 메쉬
	axe->texture = tex[2]; // 텍스쳐

	return axe;
}

GameObject* GameScene::CreateItem_Shoes(int* index_list, GLuint* tex, GLuint* vao)
{
	auto shoes = CreateEmpty();

	shoes->AddComponent<Transform3D>();
	shoes->GetComponent<Transform3D>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);
	shoes->AddComponent<Collide>();
	shoes->GetComponent<Collide>()->BoundBox = BoundBox[Shoes];
	shoes->GetComponent<Collide>()->BoundBox.pos = shoes->GetComponent<Transform3D>()->position;
	shoes->AddComponent<ItemRotate>();

	// render 부분
	shoes->modelLocation = modelLocation;
	shoes->num_index = index_list[Shoes]; // load()
	shoes->VAO = vao[Shoes]; // 메쉬
	shoes->texture = tex[2]; // 텍스쳐

	return shoes;
}

GameObject* GameScene::CreateCannon(int* index_list, GLuint* tex, GLuint* vao)
{
	auto cannon = CreateEmpty();

	cannon->AddComponent<Transform3D>();
	cannon->GetComponent<Transform3D>()->scale = glm::vec3(2.0f, 2.0f, 2.0f);
	cannon->AddComponent<CannonShot>();
	//cannon->AddComponent<Collide>();
	//cannon->GetComponent<Collide>()->BoundBox = BoundBox[Cannon];
	//cannon->GetComponent<Collide>()->BoundBox.pos = cannon->GetComponent<Transform3D>()->position;

	// render 부분
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

	// render 부분
	grass->modelLocation = modelLocation;
	grass->num_index = index_list[Grass]; //
	grass->VAO = vao[Grass]; //
	grass->texture = tex[5]; // 

	return grass;
}

GameObject* GameScene::CreateHot(int* index_list, GLuint* tex, GLuint* vao)
{
	auto hot = CreateEmpty();

	hot->AddComponent<Transform3D>();
	hot->GetComponent<Transform3D>()->roll = -90.0f;
	hot->GetComponent<Transform3D>()->scale = glm::vec3(5.0f, 5.0f, 0.5f);
	hot->AddComponent<LavaMove>();

	// render 부분
	hot->modelLocation = modelLocation;
	hot->num_index = index_list[Grass]; //
	hot->VAO = vao[Grass]; //
	hot->texture = tex[17]; // 

	return hot;
}

GameObject* GameScene::CreateBook(int* index_list, GLuint* tex, GLuint* vao)
{
	auto book = CreateEmpty();

	book->AddComponent<Transform3D>();
	book->GetComponent<Transform3D>()->scale = glm::vec3(5.0f, 5.0f, 0.5f);
	book->AddComponent<Collide>();
	book->GetComponent<Collide>()->BoundBox = BoundBox[Book];
	book->GetComponent<Collide>()->BoundBox.pos = book->GetComponent<Transform3D>()->position;


	// render 부분
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
	spike->GetComponent<Transform3D>()->scale = glm::vec3(0.7f, 0.7f, 0.7f);
	spike->AddComponent<Collide>();
	spike->GetComponent<Collide>()->BoundBox = BoundBox[Spike];
	spike->GetComponent<Collide>()->BoundBox.pos = spike->GetComponent<Transform3D>()->position;

	// render 부분
	spike->modelLocation = modelLocation;
	spike->num_index = index_list[Spike]; //
	spike->VAO = vao[Spike]; //
	spike->texture = tex[2]; // 

	return spike;
}

GameObject* GameScene::CreateBall(int* index_list, GLuint* tex, GLuint* vao)
{
	auto ball = CreateAirBox(index_list, tex, vao);

	ball->modelLocation = modelLocation;
	ball->num_index = index_list[Ball]; // load() 첫 번째
	ball->VAO = vao[Ball]; // 사각형 메쉬
	ball->texture = tex[14]; // 

	return ball;
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
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 20.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;
		plane->GetComponent<Transform3D>()->roll = 90.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[8]; // top
		if (n_scene == 5 || n_scene == 6) {
			plane->texture = tex[22];
		}
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, -19.88f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[9]; // left
		if (n_scene == 5 || n_scene == 6) {
			plane->texture = tex[23];
		}
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(19.88f, 0.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[10]; // front
		if (n_scene == 5 || n_scene == 6) {
			plane->texture = tex[24];
		}
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, 0.0f, 19.88f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = 180.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[11]; // right
		if (n_scene == 5 || n_scene == 6) {
			plane->texture = tex[25];
		}
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(-19.88f, 0.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = 90.0f;

		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[12]; // back
		if (n_scene == 5 || n_scene == 6) {
			plane->texture = tex[26];
		}
	}
	{
		auto plane = CreateEmpty();

		plane->AddComponent<Transform3D>();
		plane->GetComponent<Transform3D>()->position = glm::vec3(0.0f, -20.0f, 0.0f);
		plane->GetComponent<Transform3D>()->scale = glm::vec3(20.0f, 20.0f, 20.0f);
		plane->GetComponent<Transform3D>()->yaw = -90.0f;
		plane->GetComponent<Transform3D>()->roll = -90.0f;


		// render 부분
		plane->modelLocation = modelLocation;
		plane->num_index = index_list[2]; // load() 첫 번째
		plane->VAO = vao[2]; // 사각형 메쉬
		plane->texture = tex[13]; // bottom
		if (n_scene == 5 || n_scene == 6) {
			plane->texture = tex[27];
		}
	}
}

void GameScene::render()
{
	Scene::render();
}

