#ifndef BLINN_H
#define BLINN_H

//
//  Blinn.h
//  CS6610
//  
//  Small struct for Blinn material
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include <GLFW/glfw3.h>
#include <cyVector.h>
#include <lodepng.h>

struct Blinn
{
	int startIndex;
	int numberOfVertices;

    cyVec3f ambient;
    cyVec3f diffuse;
    cyVec3f specular;
    cyVec3f specularReflection;
    int sampleMirror;

    GLuint texAmbient;
    GLuint texDiffuse;
    GLuint texSpecular;

    Blinn()
    {
    	startIndex = 0;
		numberOfVertices = 0;

	    ambient = cyVec3f(0.1, 0.2, 0.1);
	    diffuse = cyVec3f(10.0/255.0, 230.0/255.0, 10.0/255.0);
	    specular = cyVec3f(1.0, 1.0, 1.0);
	    specularReflection = cyVec3f(0.0, 0.0, 0.0);
	    sampleMirror = 0;

	    texAmbient = 0;
	    texDiffuse = 0;
	    texSpecular = 0;
    }

    // Helper function that loads the texture for the material
	void LoadTexture(std::string texturePath, GLuint* textureID)
	{
		// Might want to process the path here

		std::vector<unsigned char> textureData;
		unsigned int width, height;
	    unsigned int error;

	    error = lodepng::decode(textureData, width, height, texturePath.c_str(), LCT_RGB);

	    if ( error != 0 ) {
	        printf("Error Loading Texture\n");
	    }

	    glGenTextures(1, textureID);
	    glBindTexture(GL_TEXTURE_2D, *(textureID));
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)textureData.data());
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} 

	// Load the texture with a GLRenderBuffer
	// void LoadTexture(GLuint* textureID, GLRenderBuffer* renderBuf)
	// {
	// 	*textureID = renderBuf->GetTextureID();
	// 	glBindTexture(GL_TEXTURE_2D, *(textureID));
	// }

    void sendTo(GLStates* glStates)
    {
    	glUniform3fv(glStates->b_ambient, 1, (const GLfloat*) &ambient);
    	glUniform3fv(glStates->b_diffuse, 1, (const GLfloat*) &diffuse);
    	glUniform3fv(glStates->b_specular, 1, (const GLfloat*) &specular);
    	glUniform3fv(glStates->b_specularReflection, 1, (const GLfloat*) &specularReflection);
    	glUniform1i(glStates->b_sampleMirror, sampleMirror);	// Toggle mirror sampling

    	glUniform1i(glStates->b_texAmbient, 0);
    	glUniform1i(glStates->b_texDiffuse, 0);
    	glUniform1i(glStates->b_texSpecular, 0);
    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, glStates->emptyTexID);

    	//Bind the texture if they exists
    	if (texAmbient > 0)
    	{
    		glUniform1i(glStates->b_texAmbient, 1);
    		glActiveTexture(GL_TEXTURE1);
	        glBindTexture(GL_TEXTURE_2D, texAmbient);
	        glGenerateMipmap(GL_TEXTURE_2D);
    	}
    	if (texDiffuse > 0)
    	{
    		glUniform1i(glStates->b_texDiffuse, 2);
    		glActiveTexture(GL_TEXTURE2);
	        glBindTexture(GL_TEXTURE_2D, texDiffuse);
	        glGenerateMipmap(GL_TEXTURE_2D);
    	}
    	if (texSpecular > 0)
    	{
    		glUniform1i(glStates->b_texSpecular, 3);
    		glActiveTexture(GL_TEXTURE3);
	        glBindTexture(GL_TEXTURE_2D, texSpecular);
	        glGenerateMipmap(GL_TEXTURE_2D);
    	}
    }
};

Blinn b;

#endif // BLINN_H
