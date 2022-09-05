#pragma once
#include "Component.h"

struct  BoundingBox
{
    float x_dimention, y_dimention, z_dimention;
};

class Collide :
    public Component
{

public:
    BoundingBox BoundBox;

public:
    void start();
    void update();
    
};

