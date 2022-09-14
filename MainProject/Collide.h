#pragma once
#include "Component.h"

struct  BoundingBox
{
    float minX, maxX, minY, maxY, minZ, maxZ;
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

