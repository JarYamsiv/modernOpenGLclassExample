#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "headers/triangle.h"
#include "headers/config.h"
#include "headers/shaders.h"
#include "headers/mesh.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    Shader traingleShader("shaders/vertex/colorTex_3d.fs", "shaders/fragment/colorTex.fs");
    //creating the wall texture
    Texture traingleTex("resources/texture/wall.jpg",traingleShader.ID,0);
    
    //triangle *T = new triangle(traingleShader.ID);
    mesh P(traingleTex.tex,traingleShader.ID, "resources/mesh/plane.dat", GL_TRIANGLES);

    //initialising projection and view matrix
    glm::mat4 projection;
    unsigned int projMatLoc;
    glm::mat4 view;
    unsigned int viewMatLoc;

    //getting the uniform location for projection and veiw matrix from vertex shader
    projMatLoc = glGetUniformLocation(traingleShader.ID, "projection");
    viewMatLoc = glGetUniformLocation(traingleShader.ID, "view");

    float screenWidth=800;
    float screenHeight=600;
    //setting up projection matrix and uploading it's value to uniform from vertexShader

    projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
    //projection = glm::mat4(1.0f);
    //projection = glm::perspective(glm::radians(45.0f),screenWidth/screenHeight,0.1f,100.0f);
    projection = glm::mat4(
        glm::vec4(3.0/4.0, 0.0, 0.0, 0.0),
        glm::vec4(    0.0, 1.0, 0.0, 0.0),
        glm::vec4(    0.0, 0.0, 0.5, 0.5),
        glm::vec4(    0.0, 0.0, 0.0, 1.0)
    );

    traingleShader.use();
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //temporarily setting veiwmatrix
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, 0.25f));
    //view = glm::mat4(1.0f);

    // render loop
    // -----------
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        traingleShader.use();
        glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(view));

        //T->Display();
        P.setRotation(-90,0.0,55.0*(float)glfwGetTime());//55.0 almost equal to 180.0/PI convertion from deg to rad
        P.Display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deleteing triangle
    //delete T;
    //delete P;
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
    return 0;
}
