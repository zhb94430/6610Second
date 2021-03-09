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
#include "Skybox.h"
#include "GLRenderBuffer.h"
#include "GLStates.h"

struct Scene
{
	std::vector<GLMesh> meshList;
	Light* l;
	Camera* cam;
	Skybox* sky;
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

	// Skybox attributes
	auto SkyModel = cyMatrix4f::Translation(scene->cam->pos);
	auto SkyMVP = Projection * View * SkyModel;

	glUniformMatrix4fv(glStates->MVP, 1, GL_FALSE, (const GLfloat*) &SkyMVP);
	glUniformMatrix4fv(glStates->M, 1, GL_FALSE, (const GLfloat*) &SkyModel);

	scene->sky->Draw();

	
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
	buffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.1, 1.0);

	DrawScene(scene, glStates);	

	// Unbind after draw calls

	buffer->Unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.4, 0.4, 1.0);
}

#endif // SCENE_H
