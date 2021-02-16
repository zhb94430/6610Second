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

#include "glfwHelpers.cpp"
#include "Project2.cpp"



// Global Variables
extern GLFWwindow* window;
extern Camera cam;
extern GLRef glRef;

std::string modelPath = "./teapot.obj";

int main(int argc, char* argv[])
{
    GLFWInit();

    auto begin = std::chrono::steady_clock::now();

    if (argc == 2)
    {
        modelPath = argv[1];
    }

    loadProgram(glRef);
	loadModel(glRef, modelPath);    

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    while (!glfwWindowShouldClose(window))
    {

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	
    	// auto current = std::chrono::steady_clock::now();
    	// auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(current - begin).count();
    	// float timeDiffF = timeDiff * 0.001;

    	// printf("Cam Pos %f %f %f\n", cam.pos[0], cam.pos[1], cam.pos[2]);

    	// MVP Matrix
    	auto Model = cyMatrix4f::Translation(-glRef.modelCenter); // Center object
    	auto Projection = cyMatrix4f::Perspective(cam.fov, 1.0, 1, 100);
    	auto View = cyMatrix4f::View(cam.pos, cam.lookAt, cam.up);
    	auto MVP = Projection * View * Model;

    	//Set Attributes and Draw buffer
    	GLuint mvp_location = glGetUniformLocation(glRef.program, "MVP");
    	GLuint vpos_location = glGetAttribLocation(glRef.program, "vertexPos");
   	
 		glBindBuffer(GL_ARRAY_BUFFER, glRef.VBO);
 		glBindVertexArray(glRef.VAO);
    
    	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &MVP);
 		
    	glEnableVertexAttribArray(vpos_location);
 		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    	glDrawArrays(GL_POINTS, 0, glRef.meshArraySize);
    	
	    glfwSwapBuffers(window);
	    glfwPollEvents();
	}
}