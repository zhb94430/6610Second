#ifndef SCENE_H
#define SCENE_H

//
//  Scene.h
//  CS6610
//  
//  Struct for scene data
//  Contains light and mesh data
//  
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include "GLMesh.h"
#include "Light.h"
#include "GLRenderBuffer.h"
#include "GLStates.h"

struct Scene
{
	std::vector<GLMesh> meshList;
	Light* l;
	Camera* cam;
};

// Draw one frame of the scene to current GL configuration
void DrawScene(Scene* scene, GLStates* glStates)
{
	// Send to GL
	glUniform3fv(glStates->cameraPos, 1, (const GLfloat*) &scene->cam->pos);
	scene->l->sendTo(glStates);

	// MVP Matrix
	auto Projection = cyMatrix4f::Perspective(scene->cam->fov, 1.0, 1, 100);
	auto View = cyMatrix4f::View(scene->cam->pos, scene->cam->lookAt, scene->cam->up);

	for (int i = 0; i < scene->meshList.size(); ++i)
	{
		auto currentMesh = &scene->meshList[i];

		auto Model = currentMesh->modelMatrix;
		auto MVP = Projection * View * Model;

		//Set Attributes
		glUniformMatrix4fv(glStates->MVP, 1, GL_FALSE, (const GLfloat*) &MVP);
		glUniformMatrix4fv(glStates->M, 1, GL_FALSE, (const GLfloat*) &Model);

		currentMesh->Draw();
	}
}

// Draw one frame of the scene to provided buffer
void DrawSceneToBuffer(Scene* scene, GLRenderBuffer* buffer, GLStates* glStates)
{
	// Bind Stuff
	
	// Save current viewport info
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); 

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer->frameBufferID);
	glViewport(0,0, buffer->width, buffer->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.1, 0.1, 0.1, 1.0);

	DrawScene(scene, glStates);	

	// Unbind after draw calls

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0,0,viewport[2],viewport[3]); //Hard code, need to change later
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.4, 0.4, 0.4, 1.0);
}

#endif // SCENE_H
