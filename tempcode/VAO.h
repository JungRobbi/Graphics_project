#pragma once
#include <gl/glew.h>

class VAO
{
public:
    static GLuint vao[100];
};

class VBO
{
public:
    static GLuint vbo_position[100];
    static GLuint vbo_normal[100];
    static GLuint vbo_uv[100];
};
