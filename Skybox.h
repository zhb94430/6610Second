#ifndef SKYBOX_H
#define SKYBOX_H

//
//  Skybox.h
//  CS6610
//  
//  Skybox with hardcoded cube and support for png load
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include <cyVector.h>
#include <lodepng.h>

class Skybox
{
public:
	Skybox();
	Skybox(std::string pathToFolder, GLStates* _glStates);

	void Draw();

	GLStates* glStates;
	cyVec3f pos; 

private:
	// GLuint cubeTextureID; // Stored in glStates
	GLuint VBO;
	GLuint VAO;
	GLuint cubemapID;

	Blinn zero;

	void LoadCubeFace(GLenum target, std::string path);
};

Skybox::Skybox()
{
	pos = cyVec3f(0.0, 0.0, 0.0);
	zero.ambient = cyVec3f(0.0, 0.0, 0.0);
	zero.diffuse = cyVec3f(0.0, 0.0, 0.0);
	zero.specular = cyVec3f(0.0, 0.0, 0.0);
	zero.specularReflection = cyVec3f(0.0, 0.0, 0.0);

	static const GLfloat points[] = {
	  -10.0f,  10.0f, -10.0f,
	  -10.0f, -10.0f, -10.0f,
	   10.0f, -10.0f, -10.0f,
	   10.0f, -10.0f, -10.0f,
	   10.0f,  10.0f, -10.0f,
	  -10.0f,  10.0f, -10.0f,
	  
	  -10.0f, -10.0f,  10.0f,
	  -10.0f, -10.0f, -10.0f,
	  -10.0f,  10.0f, -10.0f,
	  -10.0f,  10.0f, -10.0f,
	  -10.0f,  10.0f,  10.0f,
	  -10.0f, -10.0f,  10.0f,
	  
	   10.0f, -10.0f, -10.0f,
	   10.0f, -10.0f,  10.0f,
	   10.0f,  10.0f,  10.0f,
	   10.0f,  10.0f,  10.0f,
	   10.0f,  10.0f, -10.0f,
	   10.0f, -10.0f, -10.0f,
	   
	  -10.0f, -10.0f,  10.0f,
	  -10.0f,  10.0f,  10.0f,
	   10.0f,  10.0f,  10.0f,
	   10.0f,  10.0f,  10.0f,
	   10.0f, -10.0f,  10.0f,
	  -10.0f, -10.0f,  10.0f,
	  
	  -10.0f,  10.0f, -10.0f,
	   10.0f,  10.0f, -10.0f,
	   10.0f,  10.0f,  10.0f,
	   10.0f,  10.0f,  10.0f,
	  -10.0f,  10.0f,  10.0f,
	  -10.0f,  10.0f, -10.0f,
	  
	  -10.0f, -10.0f, -10.0f,
	  -10.0f, -10.0f,  10.0f,
	   10.0f, -10.0f, -10.0f,
	   10.0f, -10.0f, -10.0f,
	  -10.0f, -10.0f,  10.0f,
	   10.0f, -10.0f,  10.0f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

Skybox::Skybox(std::string pathToFolder, GLStates* _glStates) : Skybox()
{
	glStates = _glStates;

	// Cubemap
	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	// Load the six images
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, (pathToFolder+"cubemap_negz.png"));
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, (pathToFolder+"cubemap_posz.png"));
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, (pathToFolder+"cubemap_posy.png"));
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, (pathToFolder+"cubemap_negy.png"));
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, (pathToFolder+"cubemap_negx.png"));
	LoadCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, (pathToFolder+"cubemap_posx.png"));

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
}

// Draw the skybox
void Skybox::Draw()
{
	glDepthMask(GL_FALSE);
	// glUseProgram(programID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	// Set up shader attributes
	zero.sendTo(glStates);
    glUniform1f(glStates->skyboxValue, 1.0);

    glVertexAttribPointer(glStates->modelPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(glStates->modelPos);

    glUniform1i(glStates->skyboxTex, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glDepthMask(GL_TRUE);
	
	glUniform1f(glStates->skyboxValue, 0.0);
	glDisableVertexAttribArray(glStates->modelPos);
}

void Skybox::LoadCubeFace(GLenum target, std::string path)
{
	// Load the six images
	std::vector<unsigned char> img;
	unsigned int width, height, error;

	// Front Back Up Down Left Right
	error = lodepng::decode(img, width, height, path, LCT_RGB);
	if ( error != 0 ) {
        printf("Error Loading Texture in Skybox.h\n");
    }
    
    glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)img.data());
}

#endif // SKYBOX_H
