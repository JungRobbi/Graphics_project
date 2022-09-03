#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>

#include "Texture.h"
#include "shader.h"
#include "FrameWork.h"
#include "objRead.cpp"
#include "VAO.h"

class GraphicsFramework :
    public Framework
{
public:

    int img = 7;
    int Imagenum = 0;
    int widthImage, heightImage, numberOfChannel = 0;

    int num_Triangle;
    int num_vertices = 3;
    int num_triangles = 1;
    int num_Sphere = 0;
    int num_cube = 0;
    int num_Gun;
    int num_Tank;
    int num_Tong;

public:
    void run();

    void InitBuffer();
    void InitTexture();
    void InitBuffer_bind(const int );
    void Initialize();

};

