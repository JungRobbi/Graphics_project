#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <wingdi.h>

#include "../Screen_Cheat/objRead.cpp"
#include "shader.h"
#include "FrameWork.h"
#include "objRead.cpp"

class GraphicsFramework :
    public Framework
{
public:
    GLuint VAO[100];
    GLuint VBO_position[100];
    GLuint VBO_normal[100];
    GLuint VBO_uv[100];

    int img = 7;
    GLuint texture[20];
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

