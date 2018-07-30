#ifndef _CONFIG_H
#define _CONFIG_H

#include "../GLAD/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>



int initWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;
extern GLFWwindow* window;

#endif