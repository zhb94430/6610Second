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
#include "Camera.h"
#include "Skybox.h"
#include "GLRenderBuffer.h"
#include "GLDepthBuffer.h"
#include "GLStates.h"

struct Scene
{
	std::vector<GLMesh*> meshList;
	Light* l;
	Camera* cam;
	Skybox* sky;
};

extern Camera lightCam;

// Draw one frame of the scene to current GL configuration
void DrawScene(Scene* scene, GLStates* glStates, GLRenderBuffer* shadowBuffer=NULL)
{
	glUseProgram(glStates->program);
	// glStates->queryTriangulateVariableLocations();
	glStates->queryVariableLocations();

	// Send to GL
	glUniform3fv(glStates->cameraPos, 1, (const GLfloat*) &scene->cam->pos);
	scene->l->sendTo(glStates);

	// MVP Matrix
	auto Projection = cyMatrix4f::Perspective(scene->cam->fov, scene->cam->renderRatio, 0.5, 50.0);
	auto View = cyMatrix4f::View(scene->cam->pos, scene->cam->lookAt, scene->cam->up);

	auto shadowMapConversion = cyMatrix4f::Translation(cyVec3f(0.5, 0.5, 0.5)) *
							   cyMatrix4f::Scale(0.5);

	auto shadowProjection = cyMatrix4f::Perspective(lightCam.fov, lightCam.renderRatio, 0.5, 50.0); // Temp workaround to get light matrices
	auto shadowView = cyMatrix4f::View(lightCam.pos, lightCam.lookAt, lightCam.up);

	if (scene->sky != NULL)
	{
		// Skybox attributes
		auto SkyModel = cyMatrix4f::Translation(scene->cam->pos);
		auto SkyMVP = Projection * View * SkyModel;

		glUniformMatrix4fv(glStates->MVP, 1, GL_FALSE, (const GLfloat*) &SkyMVP);
		glUniformMatrix4fv(glStates->M, 1, GL_FALSE, (const GLfloat*) &SkyModel);

		scene->sky->Draw();
	}
	else
	{
		glUniform1i(glStates->skyboxTex, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glStates->emptyTexID);
	}
	
	for (int i = 0; i < scene->meshList.size(); ++i)
	{
		auto currentMesh = scene->meshList[i];

		auto Model = currentMesh->modelMatrix;
		auto VP = Projection * View;
		auto MVP = Projection * View * Model;
		auto shadowMVP = shadowMapConversion * shadowProjection * shadowView * Model;

		//Set Attributes
		glUniformMatrix4fv(glStates->MVP, 1, GL_FALSE, (const GLfloat*) &MVP);
		glUniformMatrix4fv(glStates->VP, 1, GL_FALSE, (const GLfloat*) &VP);
		glUniformMatrix4fv(glStates->M, 1, GL_FALSE, (const GLfloat*) &Model);
		glUniformMatrix4fv(glStates->shadowMVP, 1, GL_FALSE, (const GLfloat*) &shadowMVP);

		if (shadowBuffer)
		{
			glUniform1i(glStates->shadowMap, 5);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, shadowBuffer->textureID);
		}
		else
		{
			glUniform1i(glStates->shadowMap, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, glStates->emptyTexID);
		}
		
		currentMesh->Draw();
	}
}

// Draw one frame of the scene to provided buffer
void DrawSceneToBuffer(Scene* scene, GLRenderBuffer* buffer, GLStates* glStates)
{
	glUseProgram(glStates->program);
	glStates->queryVariableLocations();

	// Bind Stuff
	buffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glClearColor(0.1, 0.1, 0.1, 1.0);
	glClearColor(1.0, 1.0, 1.0, 1.0); 

	DrawScene(scene, glStates);	

	// Unbind after draw calls

	buffer->Unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glClearColor(0.4, 0.4, 0.4, 1.0);
	glClearColor(0.3, 0.3, 0.3, 1.0); 
}

// Is this really necessary?
void DrawSceneToDepthBuffer(Scene* scene, GLDepthBuffer* buffer, GLStates* glStates)
{
	// Set Program to shadow shader
	glUseProgram(glStates->program);
	glStates->queryShadowVariableLocations();

	// Bind and set up the buffer
	buffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0); // Might want to change this later

	DrawScene(scene, glStates);	

	// Unbind
	buffer->Unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0); // Might want to change this later
}

#endif // SCENE_H
