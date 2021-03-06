#ifndef GL_RENDER_BUFFER_H
#define GL_RENDER_BUFFER_H

//
//  GLRenderBuffer.h
//  CS6610
//  
//  Class for render buffer
//  
//  Record the GL buffer references
//  and make draw calls on the scene
//  
//  This class handles all GL buffer related
//  calls. Scene data should be provided separately.
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include <GLFW/glfw3.h>

#include "GLStates.h"
#include "Camera.h"

class GLRenderBuffer
{
public:
	GLRenderBuffer();
	GLRenderBuffer(GLStates* _glStates);

	// void SetWidth(int _width) {width = _width;};
	// void SetHeight(int _height) {height = _height;};
	// GLuint GetTextureID() {return textureID;};

	int width;
	int height;

	GLStates* glStates;

	// Local GL States
	// Updated to *glStates upon Draw()
	GLuint textureID;
	GLuint frameBufferID;
	GLuint depthBufferID;

	GLenum drawBufferArray[1] = {GL_COLOR_ATTACHMENT0};
};

GLRenderBuffer::GLRenderBuffer()
{
	width = 1024;
	height = 1024;
	glStates = NULL;
	textureID = 0;
	depthBufferID = 0;
}

GLRenderBuffer::GLRenderBuffer(GLStates* _glStates) : GLRenderBuffer()
{
	glStates = _glStates;
	GLfloat afLevel;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &afLevel);

	// GL Stuff
	
	// Frame Buffer
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	// Texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, afLevel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);

	// Depth Buffer
	glGenRenderbuffers(1, &depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // Link the buffers together
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);

    // ??
    glDrawBuffers(1, drawBufferArray);
    
    auto error = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (error != GL_FRAMEBUFFER_COMPLETE) {
        printf("Failed to initialize render buffer");
    }


}



#endif // GL_RENDER_BUFFER_H
