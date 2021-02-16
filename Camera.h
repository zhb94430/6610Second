#ifndef CAMERA_H
#define CAMERA_H

//
//  Camera.h
//  CS6610
//  
//  Small struct for camera stuff
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include <GLFW/glfw3.h>
#include <cyTriMesh.h>
#include <cyPoint.h>

struct Camera
{
	cyPoint3f pos;
	cyPoint3f lookAt;
	cyPoint3f up;
	float fov;
};

Camera cam = {
		    .up = cyPoint3f(0, 1, 0),
		  	.pos = cyPoint3f(10, 10, 0),
		  	.lookAt = cyPoint3f(0, 0, 0),
		  	.fov = 1.13446 // 65 Degrees
		  };

#endif // CAMERA_H