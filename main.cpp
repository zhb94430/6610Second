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
#include <OpenGL/glext.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <chrono>

#include <cyMatrix.h>

#include "glfwHelpers.cpp"
#include "Camera.h"
#include "GLStates.h"
#include "Light.h"
#include "GLMesh.h"
#include "Scene.h"

// Global Variables
extern GLFWwindow* window;
extern Camera firstCam;
extern Camera secondCam;
extern GLStates glStates;

// Scene Variables
extern Light l;

std::string modelPath = "./yoda/yoda.obj";

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
	GLRenderBuffer firstBuffer(&glStates);
	GLMesh mainMesh;
	// Special case for the broken yoda model
    if (modelPath.find("yoda.obj"))
    {
    	mainMesh = GLMesh(modelPath, &glStates, cyMatrix4f::Scale(0.001));
    }
    // Center all other models 
    else
    {
		mainMesh = GLMesh(modelPath, &glStates, cyMatrix4f::Identity()); 
		mainMesh.Center();
    }

    GLMesh quadMesh;
    quadMesh = GLMesh("./quad.obj", &glStates, &firstBuffer);

    Scene firstScene;
    firstScene.l = &l;
    firstScene.cam = &firstCam;
    firstScene.meshList.push_back(mainMesh);

    Scene secondScene;
    secondScene.l = &l;
    secondScene.cam = &secondCam;
    secondScene.meshList.push_back(quadMesh);



    // Camera
	// glUniform3fv(glStates.cameraPos, 1, (const GLfloat*) &cam.pos);
	// // Light
	// l.sendTo(&glStates);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3, 0.3, 0.3, 1.0);

    while (!glfwWindowShouldClose(window))
    {

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	
    	// auto current = std::chrono::steady_clock::now();
    	// auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin).count();
    	// float timeDiffF = timeDiff * 0.001;

    	DrawSceneToBuffer(&firstScene, &firstBuffer, &glStates);

    	DrawScene(&secondScene, &glStates);

	    glfwSwapBuffers(window);
	    glfwPollEvents();
	}
}