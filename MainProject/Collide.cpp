#include "Collide.h"
#include "Gravity.h"
#include "Scene.h"
#include "Camera.h"
#include "DestroyEffect.h"
#include <cmath>

extern bool key[256];


static BoundingBox BoundBox_Cube{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static BoundingBox BoundBox_Star{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

void Collide::start()
{
	BoundBox.pos = gameObject->GetComponent<Transform3D>()->position;
}

void Collide::update()
{
	BoundBox.pos = gameObject->GetComponent<Transform3D>()->position;
	if (gameObject == Scene::scene->p_player) {
		sub_BoundBox.pos = gameObject->GetComponent<Transform3D>()->position;
	}


	for (auto obj : Scene::scene->gameObjects) {
		if (!(obj->GetComponent<Collide>() && (obj != gameObject))) 
			continue;

		if (gameObject == Scene::scene->p_player) {
			if (glm::distance(obj->GetComponent<Transform3D>()->position, gameObject->GetComponent<Transform3D>()->position) > 5.0f)
				continue;

			if (CheckBoxtoBox(sub_BoundBox, obj->GetComponent<Collide>()->BoundBox)) {
				if (obj->VAO == Scene::scene->p_vao[Pickaxe]) {
					gameObject->Item_bag.push_back(Pickaxe);
					Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Shoes]) {
					gameObject->Item_bag.push_back(Shoes);
					Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Ball]) {
					gameObject->Item_bag.push_back(Ball);
					//Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Cube] && obj->GetComponent<DestroyEffect>()) {
					auto p = find(gameObject->Item_bag.begin(), gameObject->Item_bag.end(), Pickaxe);
					if (p != gameObject->Item_bag.end()) {
						obj->GetComponent<DestroyEffect>()->destroy = true;
						gameObject->Item_bag.erase(p);
					}
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Star]) {
					gameObject->Item_bag.push_back(Star);
					Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Spike]) {
					gameObject->Item_bag.push_back(Spike);
					Scene::scene->PushDelete(obj);
					continue;
				}

				gameObject->GetComponent<Transform3D>()->velocity.y = -gameObject->GetComponent<Gravity>()->graviti_acceleration.y;
				if (sub_BoundBox.pos.y + sub_BoundBox.minY < obj->GetComponent<Collide>()->BoundBox.pos.y + obj->GetComponent<Collide>()->BoundBox.maxY
					&& BoundBox.pos.y + BoundBox.minY >= obj->GetComponent<Collide>()->BoundBox.pos.y + obj->GetComponent<Collide>()->BoundBox.maxY) {
					auto y = obj->GetComponent<Collide>()->BoundBox.pos.y + obj->GetComponent<Collide>()->BoundBox.maxY
						- (sub_BoundBox.pos.y + sub_BoundBox.minY);
					gameObject->GetComponent<Transform3D>()->position.y += y;
				}
			}
			else if (CheckBoxtoBox(BoundBox, obj->GetComponent<Collide>()->BoundBox)) {
				if (obj->VAO == Scene::scene->p_vao[Pickaxe]) {
					gameObject->Item_bag.push_back(Pickaxe);
					Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Shoes]) {
					gameObject->Item_bag.push_back(Shoes);
					Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Ball]) {
					gameObject->Item_bag.push_back(Ball);
					//Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Cube] && obj->GetComponent<DestroyEffect>()) {
					auto p = find(gameObject->Item_bag.begin(), gameObject->Item_bag.end(), Pickaxe);
					if (p != gameObject->Item_bag.end()) {
						obj->GetComponent<DestroyEffect>()->destroy = true;
						gameObject->Item_bag.erase(p);
						continue;
					}
				}
				else if (obj->VAO == Scene::scene->p_vao[Star]) {
					gameObject->Item_bag.push_back(Star);
					Scene::scene->PushDelete(obj);
					continue;
				}
				else if (obj->VAO == Scene::scene->p_vao[Spike]) {
					gameObject->Item_bag.push_back(Spike);
					Scene::scene->PushDelete(obj);
					continue;
				}

				if (key['a']) {						// 위로 이동
					Scene::scene->p_player->GetComponent<Transform3D>()->position.x -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->position.z += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.x -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.z += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
				}
				if (key['d']) {						// 아래로 이동
					Scene::scene->p_player->GetComponent<Transform3D>()->position.x += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->position.z -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.x += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.z -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
				}
				if (key['s']) {						// 왼쪽으로 이동
					Scene::scene->p_player->GetComponent<Transform3D>()->position.x += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->position.z += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.x += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.z += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
				}
				if (key['w']) {						// 오른쪽으로 이동
					Scene::scene->p_player->GetComponent<Transform3D>()->position.x -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->position.z -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.x -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
					Scene::scene->p_player->GetComponent<Transform3D>()->direction.z -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.04;
				}
			}
		}
		else {
			if (gameObject->VAO == Scene::scene->p_vao[Star] || gameObject->VAO == Scene::scene->p_vao[Pickaxe] || gameObject->VAO == Scene::scene->p_vao[Shoes] || obj->VAO == Scene::scene->p_vao[Ball])
				continue;
			else if (obj->VAO == Scene::scene->p_vao[Star] || obj->VAO == Scene::scene->p_vao[Pickaxe] || obj->VAO == Scene::scene->p_vao[Shoes] || obj->VAO == Scene::scene->p_vao[Ball])
				continue;

			if (CheckBoxtoBox(BoundBox, obj->GetComponent<Collide>()->BoundBox)) {
				if (gameObject->GetComponent<Transform3D>()->position.y + gameObject->GetComponent<Collide>()->BoundBox.minY
					> obj->GetComponent<Transform3D>()->position.y + gameObject->GetComponent<Collide>()->BoundBox.minY) {
					gameObject->GetComponent<Transform3D>()->velocity.y = 0;
				}
				else if (obj->GetComponent<Transform3D>()->position.y + obj->GetComponent<Collide>()->BoundBox.minY
					> gameObject->GetComponent<Transform3D>()->position.y + gameObject->GetComponent<Collide>()->BoundBox.minY) {
					obj->GetComponent<Transform3D>()->velocity.y = 0;
				}
			}
		}
	}
}

bool Collide::CheckBoxtoBox(BoundingBox& a, BoundingBox& b)
{
	if (b.pos.x + b.minX < a.pos.x + a.maxX && a.pos.x + a.minX < b.pos.x + b.maxX &&
		b.pos.y + b.minY < a.pos.y + a.maxY && a.pos.y + a.minY < b.pos.y + b.maxY &&
		b.pos.z + b.minZ < a.pos.z + a.maxZ && a.pos.z + a.minZ < b.pos.z + b.maxZ
		) return true;
	return false;
}