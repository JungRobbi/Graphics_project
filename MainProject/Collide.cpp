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
	for (auto& obj : Scene::scene->gameObjects) {
		if (obj->GetComponent<Collide>()) {
			if (CheckBoxtoBox(BoundBox, obj->GetComponent<Collide>()->BoundBox)) {
				gameObject->GetComponent<Gravity>()->floor = obj->GetComponent<Collide>()->BoundBox.maxY;
				
			}
			//printf("%f", BoundBox.maxX);
		}
	}
}

bool Collide::CheckBoxtoBox(BoundingBox& a, BoundingBox& b)
{
	if (b.minX < a.maxX && a.minX < b.maxX &&
		b.minY < a.maxY && a.minY < b.maxY &&
		b.minZ < a.maxZ && a.minZ < b.maxZ
		) return true;
	return false;
}