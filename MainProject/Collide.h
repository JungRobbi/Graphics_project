#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"

struct  BoundingBox
{
    float minX, maxX, minY, maxY, minZ, maxZ;
    glm::vec3 pos;


    BoundingBox operator*(float rhs)
    {
        return BoundingBox{ minX * rhs, maxX * rhs,minY * rhs, maxY * rhs,minZ * rhs, maxZ * rhs };
    }
};

class Collide :
    public Component
{

public:
    BoundingBox BoundBox;

public:
    void start();
    void update();
    bool CheckBoxtoBox(BoundingBox& a, BoundingBox& b);
    
};

