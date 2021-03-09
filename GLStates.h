#ifndef GL_STATES_H
#define GL_STATES_H

//  GLStates.h
//  CS6610
//  
//  Simple struct for GL attributes bookkeeping.
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include <GLFW/glfw3.h>
#include <vector>

struct GLStates
{
	// Data
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint program = 0;




	// Vertex Shader
	GLuint MVP = 0;
	GLuint M = 0;
	GLuint modelPos = 0;
	GLuint modelNor = 0;
	GLuint modelUV = 0;




	// Fragment Shader
	// Camera
	GLuint cameraPos = 0;

	// Light Struct 
	GLuint l_pos = 0;
	GLuint l_color = 0;
	GLuint l_power = 0;

	// Blinn Struct
	GLuint b_ambient = 0;
	GLuint b_diffuse = 0;
	GLuint b_specular = 0;
	GLuint b_specularReflection = 0;
	GLuint b_texAmbient = 0;
	GLuint b_texDiffuse = 0;
	GLuint b_texSpecular = 0;
	GLuint b_sampleMirror = 0;

	// Skybox
	GLuint skyboxValue = 0;
	GLuint skyboxTex = 0;

	

	void queryVariableLocations()
	{
		MVP = glGetUniformLocation(program, "MVP");
	    M = glGetUniformLocation(program, "M");
	    modelPos = glGetAttribLocation(program, "modelPos");
	    modelNor = glGetAttribLocation(program, "modelNor");
	    modelUV = glGetAttribLocation(program, "modelUV");
	    
		cameraPos = glGetUniformLocation(program, "cameraPos");

	    l_pos = glGetUniformLocation(program, "l.pos");
	    l_color = glGetUniformLocation(program, "l.color");
	    l_power = glGetUniformLocation(program, "l.power");

	    b_ambient = glGetUniformLocation(program, "b.ambient");
	    b_diffuse = glGetUniformLocation(program, "b.diffuse");
	    b_specular = glGetUniformLocation(program, "b.specular");
	    b_specularReflection = glGetUniformLocation(program, "b.specularReflection");
	    b_texAmbient = glGetUniformLocation(program, "b_texAmbient");
	    b_texDiffuse = glGetUniformLocation(program, "b_texDiffuse");
	    b_texSpecular = glGetUniformLocation(program, "b_texSpecular");
	    b_sampleMirror = glGetUniformLocation(program, "b.sampleMirror");

	    skyboxValue = glGetUniformLocation(program, "skyboxValue");
		skyboxTex = glGetUniformLocation(program, "skyboxTex");	
	}
};

GLStates glStates;

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
// Obtain GLuint locations for all shader attributes
void loadProgram(GLStates* glStates)
{
	// Main Shader
	auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("./VertexShader.glsl"));
    auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./FragShader.glsl"));
    
    // Program
    glStates->program = glCreateProgram();
    glAttachShader(glStates->program, vertexShader);
    glAttachShader(glStates->program, fragShader);
    glLinkProgram(glStates->program);
    glUseProgram(glStates->program);

    // Store Variable References
    glStates->queryVariableLocations();
}

#endif // GL_STATES_H
