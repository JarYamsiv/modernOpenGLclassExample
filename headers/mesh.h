#ifndef _MESH_H
#define _MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <vector>

using std::vector;


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
    vector<float >          vertexData;

    int                     nIndex;
    vector<unsigned int>    indexData;

    vector<float>           color;
    vector<float>           finalBuffer;
    vector<float>           texCord;

    float                   tempData[12];
};

#endif