#pragma once
#include <gl/glew.h>

class VAO
{
public:
    static GLuint vao[100];
    
    static void initialize()
    {
        for (int i{}; i < 100; ++i)
            vao[i] = 0;
    }
};

class VBO
{
public:
    static GLuint vbo_position[100];
    static GLuint vbo_normal[100];
    static GLuint vbo_uv[100];

    static void initialize()
    {
        for (int i{}; i < 100; ++i) {
            vbo_position[i] = 0;
            vbo_normal[i] = 0;
            vbo_uv[i] = 0;
        }
    }
};
