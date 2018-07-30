#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "../GLAD/glad.h"
#include <GLFW/glfw3.h>


class triangle2D{
public:
                            triangle2D();
                            ~triangle2D();

    void                    Display(int);

private:
    unsigned int            VAO,VBO;
    float                   vertexData[9];
};

#endif