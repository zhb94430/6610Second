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

#include "Camera.h"
#include "Light.h"
#include "GLStates.h"

extern GLStates glStates;
extern Camera firstCam;
extern Camera mainCam;
extern Camera mirrorCam;
extern Camera lightCam;
extern Light l;

GLFWwindow* window;

bool MR_PRESSED = false;
bool ML_PRESSED = false;
bool CTL_PRESSED = false;
bool ALT_PRESSED = false;

double xposPrev = 0.0;
double yposPrev = 0.0;

// Sensitivities for camera control
double distSensitivity = 0.001;
double camAngleSensitivity = 0.001;
double lightAngleSensitivity = 0.001;

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
                loadProgram(&glStates);
                printf("Shader Recompiled\n");
                break;

            case GLFW_KEY_LEFT_CONTROL:
                CTL_PRESSED = true;
                break;

            case GLFW_KEY_RIGHT_CONTROL:
                CTL_PRESSED = true;
                break;

            case GLFW_KEY_LEFT_ALT:
                ALT_PRESSED = true;
                break;

            case GLFW_KEY_RIGHT_ALT:
            ALT_PRESSED = true;
            break;
		}
	}

    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
            case GLFW_KEY_LEFT_CONTROL:
                CTL_PRESSED = false;
                break;

            case GLFW_KEY_RIGHT_CONTROL:
                CTL_PRESSED = false;
                break;

            case GLFW_KEY_LEFT_ALT:
                ALT_PRESSED = false;
                break;

            case GLFW_KEY_RIGHT_ALT:
                ALT_PRESSED = false;
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
        if (ALT_PRESSED)
        {
            auto lookAt2Pos = firstCam.pos - firstCam.lookAt;
            double scale = 1.0 + distSensitivity * y_delta;
            firstCam.pos = firstCam.lookAt + lookAt2Pos * scale;
        }
        else if (CTL_PRESSED)
        {
            auto lookAt2Pos = lightCam.pos - lightCam.lookAt;
            double scale = 1.0 + distSensitivity * y_delta;
            lightCam.pos = lightCam.lookAt + lookAt2Pos * scale;
            l.pos = lightCam.pos;
            l.update();
        }
        else
        {
            auto lookAt2Pos = mainCam.pos - mainCam.lookAt;
            auto lookAt2PosMirror = mirrorCam.pos - mirrorCam.lookAt;
            double scale = 1.0 + distSensitivity * y_delta;
            mainCam.pos = mainCam.lookAt + lookAt2Pos * scale;
            mirrorCam.pos = cyPoint3f(mainCam.pos[0], -mainCam.pos[1], mainCam.pos[2]);
        }
    }

    // Calculate new camera pos based on x_delta & y_delta
    if (ML_PRESSED)
    {
        if (CTL_PRESSED)
        {
            // Rotate in X and Y
            auto xRot = cyMatrix3f::RotationX(y_delta * lightAngleSensitivity);
            auto yRot = cyMatrix3f::RotationY(x_delta * lightAngleSensitivity);

            l.pos = xRot * yRot * l.pos; 
            lightCam.pos = l.pos;
            l.update();
        }
        else if (ALT_PRESSED)
        {
            // Rotate in X and Y
            auto xRot = cyMatrix3f::RotationX(y_delta * camAngleSensitivity);
            auto yRot = cyMatrix3f::RotationY(x_delta * camAngleSensitivity);

            firstCam.pos = xRot * yRot * firstCam.pos; 
        }
        else 
        {
            // Rotate in X and Y
            auto xRot = cyMatrix3f::RotationX(y_delta * camAngleSensitivity);
            auto yRot = cyMatrix3f::RotationY(x_delta * camAngleSensitivity);
            // auto xRotNeg = cyMatrix3f::RotationX(-y_delta * camAngleSensitivity);
            // auto yRotNeg = cyMatrix3f::RotationY(-x_delta * camAngleSensitivity);

            mainCam.pos = xRot * yRot * mainCam.pos; 
            mirrorCam.pos = cyPoint3f(mainCam.pos[0], -mainCam.pos[1], mainCam.pos[2]);
        }
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
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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