#include "Collide.h"
#include "Gravity.h"
#include "Scene.h"


static BoundingBox BoundBox_Cube{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static BoundingBox BoundBox_Star{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

void Collide::start()
{
}

void Collide::update()
{
	BoundBox.pos = gameObject->GetComponent<Transform3D>()->position;
	for (auto obj : Scene::scene->gameObjects) {
		if (obj->GetComponent<Collide>()&&(obj!=gameObject)) {
			if (CheckBoxtoBox(BoundBox, obj->GetComponent<Collide>()->BoundBox)) {
				gameObject->GetComponent<Gravity>()->floor = obj->GetComponent<Collide>()->BoundBox.pos.y+obj->GetComponent<Collide>()->BoundBox.maxY;
				printf("Ãæµ¹");
			}
			
		}
	}
	printf("%f\n", BoundBox.pos.x);
}

bool Collide::CheckBoxtoBox(BoundingBox& a, BoundingBox& b)
{
	if (b.pos.x + b.minX < a.pos.x + a.maxX && a.pos.x + a.minX < b.pos.x + b.maxX &&
		b.pos.y + b.minY < a.pos.y + a.maxY && a.pos.y + a.minY < b.pos.y + b.maxY &&
		b.pos.z + b.minZ < a.pos.z + a.maxZ && a.pos.z + a.minZ < b.pos.z + b.maxZ
		) return true;
	return false;
}