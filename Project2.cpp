#ifndef PROJECT2_CPP
#define PROJECT2_CPP

//
//  Project2.cpp
//  CS6610
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.


//  Contains code for Project 2 submission

#include <GLFW/glfw3.h>
#include <cyTriMesh.h>
#include <cyPoint.h>

struct GLRef
{
	GLuint VAO;
	GLuint VBO;
	GLuint program;

	int meshArraySize;
	cyPoint3f modelCenter;
};

struct Camera
{
	cyPoint3f pos;
	cyPoint3f lookAt;
	cyPoint3f up;
	float fov;
};

GLRef glRef = { 0, 0, 0, 0};
Camera cam = {
		    .up = cyPoint3f(0, 1, 0),
		  	.pos = cyPoint3f(10, 10, 0),
		  	.lookAt = cyPoint3f(0, 0, 0),
		  	.fov = 1.13446 // 65 Degrees
		  };

// Load OBJ & Bind VAO/VBO
void loadModel(GLRef& glRef, std::string path)
{
	std::vector<cyPoint3f> posArray;

	// Process Mesh
	auto mesh = cyTriMesh();
	mesh.LoadFromFileObj(path.c_str());

	mesh.ComputeBoundingBox();

	for (int i = 0; i < mesh.NF(); i++)
	{
		auto posFace = mesh.F(i);

		// Three Vertices per Face
		posArray.push_back(mesh.V(posFace.v[0]));
		posArray.push_back(mesh.V(posFace.v[1]));
		posArray.push_back(mesh.V(posFace.v[2]));
	}

	glRef.meshArraySize = posArray.size();
	glRef.modelCenter = mesh.GetBoundMin() + (mesh.GetBoundMax() - mesh.GetBoundMin())/2;

	// Generate & Bind Objects
	glGenBuffers(1, &glRef.VBO);

	// Config VBO
	glBindBuffer(GL_ARRAY_BUFFER, glRef.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*posArray.size(), posArray.data(), GL_STATIC_DRAW);

	// Config VAO
	glGenVertexArrays(1, &glRef.VAO);
	glBindVertexArray(glRef.VAO);
}

GLuint LoadShader(GLenum shaderType, std::string path)
{
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile(path.c_str());
    
    if (sourceFile) {
        shaderString.assign( (std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
    }
    
    // Load and Compile Shader
    shaderID = glCreateShader(shaderType);
    
    const GLchar* shaderSource = shaderString.c_str();
    glShaderSource(shaderID, 1, (const GLchar**) &shaderSource, NULL);
    
    glCompileShader(shaderID);
    
    // Error Check
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
    
    if (shaderCompiled != GL_TRUE) {
        printf("Unable to compile shader %d\n", shaderID);
        
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        
        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);
        
        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        
        glDeleteShader(shaderID);
        shaderID = 0;
    }
    
    return shaderID;
}

// Load & Compile Shaders
void loadProgram(GLRef& glRef)
{
	// Main Shader
    // auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("/Users/Peter/GitRepos/6610Second/VertexShader.glsl"));
    // auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("/Users/Peter/GitRepos/6610Second/FragShader.glsl"));
    
	auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("./VertexShader.glsl"));
    auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./FragShader.glsl"));
    
    // Program
    glRef.program = glCreateProgram();
    glAttachShader(glRef.program, vertexShader);
    glAttachShader(glRef.program, fragShader);
    glLinkProgram(glRef.program);
    glUseProgram(glRef.program);
}

#endif // PROJECT2_CPP