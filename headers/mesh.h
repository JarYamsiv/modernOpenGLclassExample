#ifndef _MESH_H
#define _MESH_H

#include "../GLAD/glad.h"
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>


class mesh{
public:
                            mesh(int,const char*);
                            ~mesh();

    void                    Display();
    void                    setFinalBuffer();

private:
    void                    loadFromFile(const char*);

    unsigned int            VAO,VBO,EBO;
    int                     shaderProgram;

    int                     n;
    float                   *vertexData;

    int                     nIndex;
    unsigned int            *indexData;

    float                   *color;
    float                   *finalBuffer;
    float                   *texCord;

    float                   tempData[12];
};

#endif