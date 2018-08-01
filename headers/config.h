#ifndef _CONFIG_H
#define _CONFIG_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>



int initWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
extern GLFWwindow* window;


#endif