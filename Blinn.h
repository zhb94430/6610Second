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

struct Blinn
{
    cyVec3f ambient;
    cyVec3f diffuse;
    cyVec3f specular;

    void sendTo(GLStates* glStates)
    {
    	glUniform3fv(glStates->b_ambient, 1, (const GLfloat*) &ambient);
    	glUniform3fv(glStates->b_diffuse, 1, (const GLfloat*) &diffuse);
    	glUniform3fv(glStates->b_specular, 1, (const GLfloat*) &specular);
    }
};

Blinn b = { 
			cyVec3f(0.1, 0.2, 0.1),
    		cyVec3f(10.0/255.0, 230.0/255.0, 10.0/255.0),
		    cyVec3f(1.0, 1.0, 1.0)
		  };

#endif // BLINN_H
