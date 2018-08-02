#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "headers/triangle.h"
#include "headers/config.h"
#include "headers/shaders.h"
#include "headers/mesh.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

class Texture
{
  public:
    Texture(const char *texturePath,int sP,int id_)
    {
        id=id_;
        shaderProg=sP;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            //std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
    ~Texture()
    {
    }
    unsigned int tex;

  private:
    int shaderProg;
    int id;
};

int main()
{

    initWindow();

    //creating a shader for traingle
    Shader traingleShader("shaders/vertex/colorTex.fs", "shaders/fragment/colorTex.fs");
    Texture traingleTex("resources/texture/wall.jpg",traingleShader.ID,0);
    
    //traingles should be made and properly deleted using pointers otherwise they'll create segmentation fault
    //triangle *T = new triangle(traingleShader.ID);
    mesh *P = new mesh(traingleTex.tex,traingleShader.ID, "resources/mesh/plane.dat", GL_TRIANGLES);
    //P->moveTo(glm::vec3(0.0,0.0,0.0));

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
        P->setRotation(0.0,0.0,(float)glfwGetTime());
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
