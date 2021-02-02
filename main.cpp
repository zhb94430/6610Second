//
//  main.cpp
//  CS6610
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.
//
//  Some Code Referenced from http://www.glfw.org/docs/latest/quick.html#quick_include

#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>

GLFWwindow* window;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
		}
	}

}

static void GLFWInit()
{
	glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 8);

    window = glfwCreateWindow(800, 600, "6610 Project", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}


int main(int argc, char* argv[])
{
    GLFWInit();

    auto begin = std::chrono::steady_clock::now();

    glClearColor(0.0, 0.0, 0.4, 1.0);

    while (!glfwWindowShouldClose(window))
    {
	    // glClearColor(0.0, 0.0, 0.4, 1.0);

    	auto current = std::chrono::steady_clock::now();

    	auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(current - begin).count();

    	glClearColor(sin(timeDiff), cos(timeDiff), sin(timeDiff), 1.0);
    	glClear(GL_COLOR_BUFFER_BIT);

	    glfwSwapBuffers(window);
	    glfwPollEvents();
	}
}