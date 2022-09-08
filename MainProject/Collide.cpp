#include "Collide.h"

static BoundingBox BoundBox_Cube{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static BoundingBox BoundBox_Star{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

void Collide::start()
{
	BoundBox.minX = 0.0f;
	BoundBox.maxX = 0.0f;
	BoundBox.minY = 0.0f;
	BoundBox.maxY = 0.0f;
	BoundBox.minZ = 0.0f;
	BoundBox.maxZ = 0.0f;
}

void Collide::update()
{

}