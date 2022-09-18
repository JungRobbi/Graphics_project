#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"

class PlayerJump :
    public Component
{
public:
    glm::vec3 jump_acceleration = glm::vec3(0.0f, 0.06f, 0.0f);

public:
    
    void start();
    void update();
};

