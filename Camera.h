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

#include "Light.h"

struct Camera
{
	cyPoint3f pos;
	cyPoint3f lookAt;
	cyPoint3f up;
	float fov;
	float renderRatio; // Viewport Width/Height
};

Camera firstCam = {
		    .up = cyPoint3f(0, 1, 0),
		  	.pos = cyPoint3f(10, 10, 0),
		  	.lookAt = cyPoint3f(0, 0, 0),
		  	.fov = 1.13446, // 65 Degrees
		  	.renderRatio = 800.0/600.0 
		  };

Camera mainCam = {
			.up = cyPoint3f(0, 1, 0),
			.pos = cyPoint3f(10, 10, 0),
			.lookAt = cyPoint3f(0, 0, 0),
			.fov = 1.13446,
			.renderRatio = 800.0/600.0 // 65 Degrees
};

Camera mirrorCam = {
			.up = mainCam.up,
			.pos = cyPoint3f(mainCam.pos[0], -mainCam.pos[1], mainCam.pos[2]),
			.lookAt = mainCam.lookAt,
			.fov = mainCam.fov,
			.renderRatio = 1024.0/1024.0
};

extern Light l;

Camera lightCam = {
			.up = cyPoint3f(0, 1, 0),
			.pos = cyPoint3f(l.pos[0], l.pos[1], l.pos[2]),
			.lookAt = cyPoint3f(0, 0, 0),
			.fov = 0.7853982, // 45 Degrees
			.renderRatio = 1024.0/1024.0
};

#endif // CAMERA_H