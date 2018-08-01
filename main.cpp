#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "headers/triangle.h"
#include "headers/config.h"
#include "headers/shaders.h"
#include "headers/mesh.h"

#include <iostream>



int main()
{
    
    initWindow();

    //creating a shader for traingle
    Shader traingleShader("shaders/vertex/noColor.fs","shaders/fragment/noColor.fs");

    //traingles should be made and properly deleted using pointers otherwise they'll create segmentation fault
    //triangle *T = new triangle(traingleShader.ID);
    mesh     *P = new mesh(traingleShader.ID,"mesh/plane/data.dat");


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //T->Display();
        P->Display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deleteing triangle
    //delete T;
    delete P;
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    
    glfwTerminate();
    return 0;
}



