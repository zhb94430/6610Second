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

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#define GLFW_INCLUDE_GLCOREARB

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>

#include <cyMatrix.h>

#include "GLStates.h"
#include "glfwHelpers.cpp"
#include "Camera.h"
#include "GLMesh.h"
#include "Light.h"
#include "Blinn.h"


// Global Variables
extern GLFWwindow* window;
extern Camera cam;
extern GLStates glStates;

// Scene Variables
extern Light l;
extern Blinn b;

std::string modelPath = "./teapot.obj";

int main(int argc, char* argv[])
{
    GLFWInit();

    auto begin = std::chrono::steady_clock::now();

    if (argc == 2)
    {
        modelPath = argv[1];
    }

    loadProgram(&glStates);
	

	// Setup scene
	GLMesh currentMesh = GLMesh(modelPath, &glStates, cyMatrix4f::Identity());
	currentMesh.Center();

	glUniform3fv(glStates.cameraPos, 1, (const GLfloat*) &cam.pos);
	l.sendTo(&glStates);
	b.sendTo(&glStates);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    while (!glfwWindowShouldClose(window))
    {

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	
    	// auto current = std::chrono::steady_clock::now();
    	// auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin).count();
    	// float timeDiffF = timeDiff * 0.001;

    	// printf("Cam Pos %f %f %f\n", cam.pos[0], cam.pos[1], cam.pos[2]);

    	// Update light
    	l.sendTo(&glStates);

    	// MVP Matrix
    	auto Projection = cyMatrix4f::Perspective(cam.fov, 1.0, 1, 100);
    	auto View = cyMatrix4f::View(cam.pos, cam.lookAt, cam.up);
    	auto MVP = Projection * View * currentMesh.modelMatrix;

    	//Set Attributes
    	glUniformMatrix4fv(glStates.MVP, 1, GL_FALSE, (const GLfloat*) &MVP);
    	glUniformMatrix4fv(glStates.M, 1, GL_FALSE, (const GLfloat*) &currentMesh.modelMatrix);

    	currentMesh.Draw();

	    glfwSwapBuffers(window);
	    glfwPollEvents();
	}
}