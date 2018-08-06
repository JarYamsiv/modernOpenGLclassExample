#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "headers/triangle.h"
#include "headers/shaders.h"
#include "headers/mesh.h"
#include "headers/Camera__.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

//======================================functions forward declaration=============================
int initWindow();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
//===========================================END==================================================

//============================================GLOBAL VARIABLES================================
// settings
GLFWwindow *window;
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

//game related global variables
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0;
float lastFrame = 0.0;
float currentFrame;

//camera testing
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float pitch = -90.0;
float yaw = 0.0;
bool isFPS=false;

//==========================================GLOBAL VARIABLES END=================================

//========================================TEXTURE CLASS==========================================
class Texture
{
  public:
    Texture(const char *texturePath, int sP, int id_)
    {
        id = id_;
        shaderProg = sP;
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
//==============================================END==============================================

int main()
{

    initWindow();

    //creating a shader for traingle
    Shader traingleShader("shaders/vertex/colorTex_3d.fs", "shaders/fragment/colorTex.fs");
    //creating the wall texture
    Texture traingleTex("resources/texture/wall.jpg", traingleShader.ID, 0);

    //triangle *T = new triangle(traingleShader.ID);
    mesh P(traingleTex.tex, traingleShader.ID, "resources/mesh/cube.dat", GL_TRIANGLES);

    //initialising projection and view matrix
    glm::mat4 projection;
    unsigned int projMatLoc;
    glm::mat4 view;
    unsigned int viewMatLoc;

    //getting the uniform location for projection and veiw matrix from vertex shader
    projMatLoc = glGetUniformLocation(traingleShader.ID, "projection");
    viewMatLoc = glGetUniformLocation(traingleShader.ID, "view");

    float screenWidth = (float)SCR_WIDTH;
    float screenHeight = (float)SCR_HEIGHT;
    //setting up projection matrix and uploading it's value to uniform from vertexShader
    projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
    traingleShader.use();
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //temporarily setting veiwmatrix
    view = glm::lookAt(glm::vec3(0.0f, 3.0f, 3.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
    //view = glm::mat4(1.0f);

    // render loop
    // -----------
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        traingleShader.use();
        glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(view));

        //P.setRotationWithVec(55.0 * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
        //P.setRotation(-55.0,0.0,55.0*(float)glfwGetTime());//55.0 almost equal to 180.0/PI convertion from deg to rad
        //P.Display();
        P.multipleRendering();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

//=============================================function definitions==============================

int initWindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif*/

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window,GLFW_KEY_SPACE)== GLFW_PRESS)
        isFPS=!isFPS;
    
    if(isFPS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    float cameraSpeed = 2.5*deltaTime; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
