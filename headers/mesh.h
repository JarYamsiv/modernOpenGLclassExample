#ifndef _MESH_H
#define _MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::vector;



class mesh{
public:
                            mesh(unsigned int,int,const char*,GLenum buildMode);
                            ~mesh();

    void                    Display();
    void                    setFinalBuffer();
    void                    moveTo(glm::vec3 newPos);
    void                    moveTo(float x,float y,float z);
    void                    setRotation(float rx,float ry,float rz);

private:
    void                    loadFromFile(const char*);

    unsigned int            VAO,VBO,EBO;
    int                     shaderProgram;
    unsigned int            texture;

    int                     n;
    vector<float >          vertexData;

    int                     nIndex;
    vector<unsigned int>    indexData;

    vector<float>           color;
    vector<float>           finalBuffer;
    vector<float>           texCord;

    GLenum                  bMode;//build mode whether it's triangle quads or etc..
    unsigned int            modelMatLoc;
    glm::vec3               position;
    float                   rotationX,rotationY,rotationZ;
};

#endif