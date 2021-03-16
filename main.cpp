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
#include "Skybox.h"

// Global Variables
extern GLFWwindow* window;
extern Camera firstCam;
extern Camera secondCam;
extern GLStates glStates;

// Scene Variables
extern Light l;

// std::string modelPath = "./yoda/yoda.obj";
std::string modelPath = "./teapot.obj";
// std::string modelPath = "./sphere.obj";

int main(int argc, char* argv[])
{
    GLFWInit();

    auto begin = std::chrono::steady_clock::now();

    if (argc == 2)
    {
        modelPath = argv[1];
    }

	loadShadowProgram(&glShadowStates);
	loadProgram(&glStates);

	glStates.generateEmptyTexture();
	glShadowStates.generateEmptyTexture();

	// Setup scene
	GLRenderBuffer mirrorBuffer(&glStates);
	GLDepthBuffer shadowBuffer(&glShadowStates);
	Skybox sky("./cubemap/", &glStates);

	GLMesh mainMesh;
	// Special case for the broken yoda model
    if (modelPath.find("yoda.obj") != std::string::npos)
    {
    	mainMesh = GLMesh(modelPath, &glStates, cyMatrix4f::Scale(0.001));
    }
    // Center all other models 
    else
    {
		// mainMesh = GLMesh(modelPath, &glStates, cyMatrix4f::Identity()); 
    	auto mainMeshModel = cyMatrix4f::Translation(cyVec3f(0.0, 1.0, 0.0)) 
    					   * cyMatrix4f::Scale(0.10)
    					   * cyMatrix4f::RotationX(-1.570796);

		mainMesh = GLMesh(modelPath, &glStates, mainMeshModel); 
		mainMesh.Center();
    }

    GLMesh quadMesh("./quad.obj", &glStates, cyMatrix4f::Scale(10), &mirrorBuffer);
    GLMesh lightMesh("./light/light.obj", &glStates, cyMatrix4f::Translation(l.pos));

    // Scene mirrorScene;
    // mirrorScene.l = &l;
    // mirrorScene.cam = &mirrorCam;
    // mirrorScene.sky = &sky;
    // mirrorScene.meshList.push_back(mainMesh);

    Scene shadowScene;
    shadowScene.l = &l;
    shadowScene.cam = &lightCam;
    shadowScene.sky = NULL;
    shadowScene.meshList.push_back(mainMesh);

    // Scene firstScene;
    // firstScene.l = &l;
    // firstScene.cam = &mainCam;
    // firstScene.sky = &sky;
    // firstScene.meshList.push_back(mainMesh);

    Scene secondScene;
    secondScene.l = &l;
    secondScene.cam = &mainCam;
    secondScene.sky = &sky;
    secondScene.meshList.push_back(quadMesh);
    secondScene.meshList.push_back(mainMesh);
    // secondScene.meshList.push_back(lightMesh);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3, 0.3, 0.3, 1.0);

    while (!glfwWindowShouldClose(window))
    {

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glClearColor(0.3, 0.3, 0.3, 1.0);  
		glClearColor(1.0, 1.0, 1.0, 1.0);   	

    	// auto current = std::chrono::steady_clock::now();
    	// auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin).count();
    	// float timeDiffF = timeDiff * 0.001;

    	// DrawSceneToBuffer(&firstScene, &firstBuffer, &glStates);
    	// DrawScene(&secondScene, &glStates);

    	// DrawScene(&firstScene, &glStates);

    	// DrawSceneToBuffer(&mirrorScene, &mirrorBuffer, &glStates);

		// DrawSceneToDepthBuffer(&shadowScene, &shadowBuffer, &glShadowStates);
		DrawSceneToBuffer(&shadowScene, &mirrorBuffer, &glShadowStates);
    	
		// Temporary code to bind the shadow buffer
		glUniform1i(glStates.shadowMap, 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, mirrorBuffer.textureID);

    	DrawScene(&secondScene, &glStates);
    	// DrawScene(&secondScene, &glStates);
    	

	    glfwSwapBuffers(window);
	    glfwPollEvents();
	}
}