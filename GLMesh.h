#ifndef GL_MESH_H
#define GL_MESH_H

//  GLMesh.h
//  CS6610
//  
//  Simple class to bridge cyTriMesh.h and this project
//  
//  This is a retake of CS6610, Spring 2018
//  My code from Spring 2018 will be referenced
//
//  Created by Peter Zhang
//  Copyright © 2021 Peter Zhang. All rights reserved.

#include <GLFW/glfw3.h>
#include <cyTriMesh.h>

#include "GLStates.h"

class GLMesh
{
public:
	cyMatrix4f modelMatrix;

	GLMesh();
	GLMesh(std::string filePath, GLStates* _glStates);
	GLMesh(std::string filePath, GLStates* _glStates, cyMatrix4f m);

	void Draw();
	void Center(); //Centers the mesh based on bounding box

private:
	// Struct for combining arrays from cyTriMesh
    struct CombinedVertex {
        cyPoint3f pos;
        cyPoint3f normal;
    };
    struct CombinedFace {
        CombinedVertex v0;
        CombinedVertex v1;
        CombinedVertex v2;
    };

    cyTriMesh mesh;
    GLStates* glStates;
    int drawSize = 0; // Keep track of how many vertices to draw

    // Local GL States
    // Updated to *glStates upon Draw()
    GLuint VBO;
    GLuint VAO;

    void Load(std::string filePath);
};

GLMesh::GLMesh()
{
	modelMatrix = cyMatrix4f::Identity();
    glStates = NULL;
    mesh = cyTriMesh();
}

GLMesh::GLMesh(std::string filePath, GLStates* _glStates)
{
	modelMatrix = cyMatrix4f::Identity();
	glStates = _glStates;

	if (glStates == NULL)
	{
		printf("glStates initialized to NULL, program behavior undefined\n");
	}

	Load(filePath);
}

GLMesh::GLMesh(std::string filePath, GLStates* _glStates, cyMatrix4f m)
{
	modelMatrix = m;
	glStates = _glStates;

	if (glStates == NULL)
	{
		printf("glStates initialized to NULL, program behavior undefined\n");
	}

	Load(filePath);
}

void GLMesh::Load(std::string filePath)
{
	mesh = cyTriMesh();
	mesh.LoadFromFileObj(filePath.c_str());

	// Process TriMesh for GL
    // Load data into one array
    std::vector<CombinedFace> combinedVector;

    for (int i = 0; i < mesh.NF(); i++) 
    {
        cyTriMesh::TriFace posFace = mesh.F(i);
        cyTriMesh::TriFace normalFace;

        cyPoint3f posV0, posV1, posV2;
        cyPoint3f normalV0, normalV1, normalV2;

        posV0 = mesh.V(posFace.v[0]);
        posV1 = mesh.V(posFace.v[1]);
        posV2 = mesh.V(posFace.v[2]);

        if (mesh.HasNormals())
        {
            normalFace = mesh.FN(i);
            normalV0 = mesh.VN(normalFace.v[0]);
            normalV1 = mesh.VN(normalFace.v[1]);
            normalV2 = mesh.VN(normalFace.v[2]);
        }
        else
        {
            printf("%s does not contain normal\n", filePath.c_str());
        }

        CombinedVertex v0 = { posV0, normalV0 };
        CombinedVertex v1 = { posV1, normalV1 };
        CombinedVertex v2 = { posV2, normalV2 };

        CombinedFace c = {v0, v1, v2};
        combinedVector.push_back(c);
    }

    // Generate VBO & VAO
	unsigned long combinedSize = sizeof(CombinedFace)*combinedVector.size();
	drawSize = combinedVector.size()*6;


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, combinedSize, combinedVector.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(glStates->modelPos, 3, GL_FLOAT, GL_FALSE, sizeof(CombinedVertex), (GLvoid*)0);
    glVertexAttribPointer(glStates->modelNor, 3, GL_FLOAT, GL_FALSE, sizeof(CombinedVertex), (GLvoid*)(3*sizeof(GL_FLOAT)));
}

void GLMesh::Draw()
{
	// Update the global *glStates
	glStates->VAO = VAO;
	glStates->VBO = VBO;

	glBindBuffer(GL_ARRAY_BUFFER, glStates->VBO);
	glBindVertexArray(glStates->VAO);

	glEnableVertexAttribArray(glStates->modelPos);
	glEnableVertexAttribArray(glStates->modelNor);

	glDrawArrays(GL_TRIANGLES, 0, drawSize);

	glDisableVertexAttribArray(glStates->modelPos);
	glDisableVertexAttribArray(glStates->modelNor);
}

void GLMesh::Center()
{
	auto modelCenter = mesh.GetBoundMin() + (mesh.GetBoundMax() - mesh.GetBoundMin())/2;

	modelMatrix = cyMatrix4f::Translation(-modelCenter);
}

#endif // GL_MESH_H

