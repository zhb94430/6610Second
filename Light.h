#ifndef LIGHT_H
#define LIGHT_H

//
//  Light.h
//  CS6610
//  
//  Small struct for light
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include <GLFW/glfw3.h>
#include <cyVector.h>
#include <cyPoint.h>

#include "GLStates.h"

struct Light 
{
    cyPoint3f pos;
    cyVec3f color;
    float power;

    void sendTo(GLStates* glStates)
    {
    	glUniform3fv(glStates->l_pos, 1, (const GLfloat*) &pos);
    	glUniform3fv(glStates->l_color, 1, (const GLfloat*) &color);
    	glUniform1fv(glStates->l_power, 1, (const GLfloat*) &power);
    }
};

Light l = { cyPoint3f(0.0, 8.0, 5.0), cyVec3f(1.0, 1.0, 1.0), 200.0 };

#endif // LIGHT_H
