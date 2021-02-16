#ifndef GLFW_HELPERS_CPP
#define GLFW_HELPERS_CPP

//
//  glfwHelpers.cpp
//  CS6610
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

// Code for various helper functions for glfw
// Including call back functions for key/mouse interactions

#define GLFW_INCLUDE_GLCOREARB

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include "Project2.cpp"

extern Camera cam;

GLFWwindow* window;

bool MR_PRESSED = false;
bool ML_PRESSED = false;

double xposPrev = 0.0;
double yposPrev = 0.0;

// Sensitivities for camera control
double distSensitivity = 0.001;
double angleSensitivity = 0.001;

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

            case GLFW_KEY_F6:
                loadProgram(glRef);
                printf("Shader Recompiled\n");
                break;
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    double x_delta = xpos - xposPrev;
    double y_delta = ypos - yposPrev;

    // Calculate new camera dist based on y_delta
    if (MR_PRESSED)
    {
        auto lookAt2Pos = cam.pos - cam.lookAt;
        double scale = 1.0 + distSensitivity * y_delta;
        cam.pos = cam.lookAt + lookAt2Pos * scale;
    }

    // Calculate new camera pos based on x_delta & y_delta
    if (ML_PRESSED)
    {
        // Rotate in X and Y
        auto xRot = cyMatrix3f::RotationX(y_delta * angleSensitivity);
        auto yRot = cyMatrix3f::RotationY(x_delta * angleSensitivity);

        cam.pos = xRot * yRot * cam.pos;
    }

    xposPrev = xpos;
    yposPrev = ypos;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            MR_PRESSED = true;    
        }

        if (action == GLFW_RELEASE)
        {
            MR_PRESSED = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            ML_PRESSED = true;    
        }

        if (action == GLFW_RELEASE)
        {
            ML_PRESSED = false;
        }
    }
}


static void GLFWInit()
{
	glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()) 
    {
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

    // Keyboard control
    glfwSetKeyCallback(window, key_callback);

    // Mouse control
    if (glfwRawMouseMotionSupported())
    {
    	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

#endif