#include "Collide.h"
#include "Gravity.h"
#include "Scene.h"
#include "Camera.h"
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
	for (auto obj : Scene::scene->gameObjects) {
		if (obj->GetComponent<Collide>()&&(obj!=gameObject)) {
			if (CheckBoxtoBox(BoundBox, obj->GetComponent<Collide>()->BoundBox)) {
				if (gameObject->GetComponent<Transform3D>()->position.y + gameObject->GetComponent<Collide>()->BoundBox.minY
					> obj->GetComponent<Transform3D>()->position.y + gameObject->GetComponent<Collide>()->BoundBox.minY) {
					gameObject->GetComponent<Transform3D>()->velocity.y = 0;
				}
				else if (obj->GetComponent<Transform3D>()->position.y + obj->GetComponent<Collide>()->BoundBox.minY
					> gameObject->GetComponent<Transform3D>()->position.y + gameObject->GetComponent<Collide>()->BoundBox.minY) {
					obj->GetComponent<Transform3D>()->velocity.y = 0;
				}
				else {
					if (gameObject != Scene::scene->p_player)
						continue;

					if (key['a']) {						// 위로 이동
						Scene::scene->p_player->GetComponent<Transform3D>()->position.x -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
						Scene::scene->p_player->GetComponent<Transform3D>()->position.z += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
					}
					if (key['d']) {						// 아래로 이동
						Scene::scene->p_player->GetComponent<Transform3D>()->position.x += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
						Scene::scene->p_player->GetComponent<Transform3D>()->position.z -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
					}
					if (key['s']) {						// 왼쪽으로 이동
						Scene::scene->p_player->GetComponent<Transform3D>()->position.x += cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
						Scene::scene->p_player->GetComponent<Transform3D>()->position.z += sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
					}
					if (key['w']) {						// 오른쪽으로 이동
						Scene::scene->p_player->GetComponent<Transform3D>()->position.x -= cos((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
						Scene::scene->p_player->GetComponent<Transform3D>()->position.z -= sin((float)glm::radians(Scene::scene->p_player->GetComponent<Camera>()->fpsy)) * 0.015;
					}
					
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