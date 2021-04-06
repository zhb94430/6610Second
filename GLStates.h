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

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <vector>

// Stores GL references to the main shader program
struct GLStates
{
	// Empty Texture ID
	GLuint emptyTexID = 0;

	// Common References
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint program = 0;

	// Vertex Shader
	GLuint MVP = 0; // In shadow shader as well
	GLuint shadowMVP = 0;
	GLuint M = 0;
	GLuint modelPos = 0; // In shadow shader as well
	GLuint modelNor = 0;
	GLuint modelUV = 0;

	// TessCtlShader
	GLuint tessLevel = 0;
	GLuint VP = 0;
	GLuint b_texDisplacement = 0;
	GLuint displacementScale = 0;

	// Fragment Shader
	// Camera
	GLuint cameraPos = 0;

	// Light Struct 
	GLuint l_pos = 0;
	GLuint l_dir = 0;
	GLuint l_color = 0;
	GLuint l_power = 0;
	GLuint l_cutoff = 0;

	// Blinn Struct
	GLuint b_ambient = 0;
	GLuint b_diffuse = 0;
	GLuint b_specular = 0;
	GLuint b_specularReflection = 0;
	GLuint b_texAmbient = 0;
	GLuint b_texDiffuse = 0;
	GLuint b_texSpecular = 0;
	GLuint b_texNormal = 0;

	GLuint b_sampleMirror = 0;

	// Skybox
	GLuint skyboxValue = 0;
	GLuint skyboxTex = 0;

	GLuint shadowMap = 0;



	void queryVariableLocations()
	{
		MVP = glGetUniformLocation(program, "MVP");
		shadowMVP = glGetUniformLocation(program, "shadowMVP");
	    M = glGetUniformLocation(program, "M");
	    modelPos = glGetAttribLocation(program, "modelPos");
	    modelNor = glGetAttribLocation(program, "modelNor");
	    modelUV = glGetAttribLocation(program, "modelUV");
	    
		cameraPos = glGetUniformLocation(program, "cameraPos");

	    l_pos = glGetUniformLocation(program, "l.pos");
	    l_dir = glGetUniformLocation(program, "l.dir");
	    l_color = glGetUniformLocation(program, "l.color");
	    l_power = glGetUniformLocation(program, "l.power");
	    l_cutoff = glGetUniformLocation(program, "l.cutoff");

	    b_ambient = glGetUniformLocation(program, "b.ambient");
	    b_diffuse = glGetUniformLocation(program, "b.diffuse");
	    b_specular = glGetUniformLocation(program, "b.specular");
	    b_specularReflection = glGetUniformLocation(program, "b.specularReflection");
	    b_texAmbient = glGetUniformLocation(program, "b_texAmbient");
	    b_texDiffuse = glGetUniformLocation(program, "b_texDiffuse");
	    b_texSpecular = glGetUniformLocation(program, "b_texSpecular");
	    b_texNormal = glGetUniformLocation(program, "b_texNormal");
	    b_sampleMirror = glGetUniformLocation(program, "b.sampleMirror");

	    skyboxValue = glGetUniformLocation(program, "skyboxValue");
		skyboxTex = glGetUniformLocation(program, "skyboxTex");	

		shadowMap = glGetUniformLocation(program, "shadowMap");	
	}

	void queryShadowVariableLocations()
	{
		// MVP = glGetUniformLocation(program, "MVP");
		// modelPos = glGetAttribLocation(program, "modelPos");

		// queryTessVariableLocations();

		queryTessVariableLocations();
		// queryTriangulateVariableLocations();
		queryVariableLocations();
	}

	void queryTriangulateVariableLocations()
	{
		MVP = glGetUniformLocation(program, "MVP");
		modelPos = glGetAttribLocation(program, "modelPos");
		cameraPos = glGetUniformLocation(program, "cameraPos");
	}

	void queryTessVariableLocations()
	{
		tessLevel = glGetUniformLocation(program, "tessLevel");
		VP = glGetUniformLocation(program, "VP");
		b_texDisplacement = glGetUniformLocation(program, "b_texDisplacement");
		displacementScale = glGetUniformLocation(program, "displacementScale");
	}

	void queryP6VariableLocations()
	{
		queryTessVariableLocations();
		// queryTriangulateVariableLocations();
		queryVariableLocations();
	}

	// Enable current GL Program
	void enable()
	{
		glUseProgram(program);
	}

	// Generate empty texture to feed the texture unit when not in use
	// Should be called when a GLStates is created
	// https://stackoverflow.com/questions/42238177/check-if-sampler2d-is-empty
	void generateEmptyTexture()
	{
		glGenTextures(1, &emptyTexID);

	    GLubyte data[] = { 255, 255, 255, 255 };

	    glBindTexture(GL_TEXTURE_2D, emptyTexID);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
};

GLStates glStates;
GLStates glShadowStates;
GLStates glStates_Triangulate;

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

void checkLinkerError(GLuint programID)
{

	// Error Check
    GLint programLinked = GL_FALSE;
    glGetProgramiv( programID, GL_LINK_STATUS, &programLinked);
    
    if (programLinked != GL_TRUE) {
        printf("Unable to link program, rendering behavior undefined %d\n", programID);
        
        GLint maxLength = 0;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
        
        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);
        
        for (int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
    }
}

void loadProgram(GLStates* glStates)
{
	auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("./VertexShader.glsl"));

	// auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("./TessVertexShader.glsl"));
	auto tessCtlShader = LoadShader(GL_TESS_CONTROL_SHADER, std::string("./TessCtlShader.glsl"));
	auto tessEvalShader = LoadShader(GL_TESS_EVALUATION_SHADER, std::string("./TessEvalShader.glsl"));
	
	// auto geometryShader = LoadShader(GL_GEOMETRY_SHADER, std::string("./GeometryShader.glsl"));
    auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./FragShader.glsl"));
    // auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./TessFragShader.glsl"));
     // auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./GeoFragShader.glsl"));

    // Program
    glStates->program = glCreateProgram();
    glAttachShader(glStates->program, vertexShader);
    glAttachShader(glStates->program, tessCtlShader);
    glAttachShader(glStates->program, tessEvalShader);
    // glAttachShader(glStates->program, geometryShader);
    glAttachShader(glStates->program, fragShader);
    glLinkProgram(glStates->program);

    checkLinkerError(glStates->program);

    glUseProgram(glStates->program);

    // glStates->queryVariableLocations();
    glStates->queryP6VariableLocations();
}

// Temporary workaround for shadow shaders
void loadShadowProgram(GLStates* glStates)
{
	// Main Shader
	auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("./VertexShader.glsl"));
   	auto tessCtlShader = LoadShader(GL_TESS_CONTROL_SHADER, std::string("./TessCtlShader.glsl"));
	auto tessEvalShader = LoadShader(GL_TESS_EVALUATION_SHADER, std::string("./TessEvalShader.glsl"));
    auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./ShadowFragShader.glsl"));
    
    // Program
    glStates->program = glCreateProgram();
    glAttachShader(glStates->program, vertexShader);
    glAttachShader(glStates->program, tessCtlShader);
    glAttachShader(glStates->program, tessEvalShader);
    glAttachShader(glStates->program, fragShader);
    glLinkProgram(glStates->program);

	checkLinkerError(glStates->program);

    glUseProgram(glStates->program);

    // Store Variable References
    glStates->queryShadowVariableLocations();
}

void loadTriangulateProgram(GLStates* glStates)
{
	// auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("./GeoVertexShader.glsl"));
	// auto geometryShader = LoadShader(GL_GEOMETRY_SHADER, std::string("./GeometryShader.glsl"));
 //    auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./GeoFragShader.glsl"));

    auto vertexShader = LoadShader(GL_VERTEX_SHADER, std::string("./VertexShader.glsl"));
    auto tessCtlShader = LoadShader(GL_TESS_CONTROL_SHADER, std::string("./TessCtlShader.glsl"));
	auto tessEvalShader = LoadShader(GL_TESS_EVALUATION_SHADER, std::string("./TessEvalShader.glsl"));
	auto geometryShader = LoadShader(GL_GEOMETRY_SHADER, std::string("./GeometryShader.glsl"));
    auto fragShader = LoadShader(GL_FRAGMENT_SHADER, std::string("./GeoFragShader.glsl"));

    glStates->program = glCreateProgram();
    glAttachShader(glStates->program, vertexShader);
    glAttachShader(glStates->program, tessCtlShader);
    glAttachShader(glStates->program, tessEvalShader);
    glAttachShader(glStates->program, geometryShader);
    glAttachShader(glStates->program, fragShader);
    glLinkProgram(glStates->program);

    checkLinkerError(glStates->program);

    glUseProgram(glStates->program);

    // glStates->queryTriangulateVariableLocations();
    glStates->queryP6VariableLocations();
}

#endif // GL_STATES_H
