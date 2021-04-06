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

#include "Blinn.h"
#include "GLStates.h"
#include "GLRenderBuffer.h"

class GLMesh
{
public:
	cyMatrix4f modelMatrix;

	GLMesh();
	GLMesh(std::string filePath, GLStates* _glStates, GLRenderBuffer* renderBuffer = NULL);
	GLMesh(std::string filePath, GLStates* _glStates, cyMatrix4f m, GLRenderBuffer* renderBuffer = NULL);

	void Draw();
	void Center(); //Centers the mesh based on bounding box

private:
	// Struct for combining arrays from cyTriMesh
    struct CombinedVertex 
    {
        cyPoint3f pos;
        cyPoint3f normal;
        cyPoint3f uv;
    };
    struct CombinedFace 
    {
        CombinedVertex v0;
        CombinedVertex v1;
        CombinedVertex v2;
    };

    cyTriMesh mesh;
    std::vector<Blinn> materials; // Store blinn materials
    GLStates* glStates;
    int drawSize = 0; // Keep track of how many vertices to draw

    // Local GL States
    // Updated to *glStates upon Draw()
    GLuint VBO;
    GLuint VAO;

    void Load(std::string filePath, GLRenderBuffer* renderBuffer = NULL);
};

GLMesh::GLMesh()
{
	modelMatrix = cyMatrix4f::Identity();
    glStates = NULL;
    mesh = cyTriMesh();
}

GLMesh::GLMesh(std::string filePath, GLStates* _glStates, GLRenderBuffer* renderBuffer)
{
	modelMatrix = cyMatrix4f::Identity();
	glStates = _glStates;

	if (glStates == NULL)
	{
		printf("glStates initialized to NULL, program behavior undefined\n");
	}

	if (renderBuffer)
	{
		Load(filePath, renderBuffer);
	}
	else
	{
		Load(filePath);
	}
	
}

GLMesh::GLMesh(std::string filePath, GLStates* _glStates, cyMatrix4f m, GLRenderBuffer* renderBuffer)
{
	modelMatrix = m;
	glStates = _glStates;

	if (glStates == NULL)
	{
		printf("glStates initialized to NULL, program behavior undefined\n");
	}

	if (renderBuffer)
	{
		Load(filePath, renderBuffer);
	}
	else
	{
		Load(filePath);
	}
}

void GLMesh::Load(std::string filePath, GLRenderBuffer* renderBuffer)
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
        cyTriMesh::TriFace uvFace;

        cyPoint3f posV0, posV1, posV2;
        cyPoint3f normalV0, normalV1, normalV2;
        cyPoint3f uvV0, uvV1, uvV2;

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

        if (mesh.HasTextureVertices())
        {
        	uvFace = mesh.FT(i);
        	uvV0 = mesh.VT(uvFace.v[0]);
            uvV1 = mesh.VT(uvFace.v[1]);
            uvV2 = mesh.VT(uvFace.v[2]);
        }
        else
        {
            printf("%s does not contain uv\n", filePath.c_str());
            uvV0 = cyPoint3f(0,0,0);
            uvV1 = cyPoint3f(0,0,0);
            uvV2 = cyPoint3f(0,0,0);
        }

        CombinedVertex v0 = { posV0, normalV0, uvV0 };
        CombinedVertex v1 = { posV1, normalV1, uvV1 };
        CombinedVertex v2 = { posV2, normalV2, uvV2 };

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
    glVertexAttribPointer(glStates->modelUV, 2, GL_FLOAT, GL_FALSE, sizeof(CombinedVertex), (GLvoid*)(6*sizeof(GL_FLOAT)));

    // Process Materials for GL
    // Prepare Path String
    // by trimming the .obj at the end
    std::size_t lastFound = filePath.rfind("/");
    std::string processedPath;
    
    if (lastFound != std::string::npos)
    {
    	processedPath = filePath.substr(0, lastFound);
    }

    int vertexCount = 0;

    // No material, generate default blinn material so I can see stuff
    if (mesh.NM() == 0)
    {
    	Blinn dummyMaterial = Blinn();
    	dummyMaterial.startIndex = 0;
    	dummyMaterial.numberOfVertices = mesh.NF()*3;

    	dummyMaterial.specularReflection = cyVec3f(0.3, 0.3, 0.3);

    	materials.push_back(dummyMaterial);
    }
    else
    {
    	for (int i = 0; i < mesh.NM(); ++i)
	    {
	    	int currentFaceCount = mesh.GetMaterialFaceCount(i);
	        cyTriMesh::Mtl currentTriMtl = mesh.M(i);

	        Blinn currentMaterial = Blinn();
	    	currentMaterial.startIndex = vertexCount;
	    	currentMaterial.numberOfVertices = currentFaceCount * 3;

	    	// Assume all have a,d,s color
	    	currentMaterial.ambient = cyVec3f(currentTriMtl.Ka[0], currentTriMtl.Ka[1], currentTriMtl.Ka[2]);
			currentMaterial.diffuse = cyVec3f(currentTriMtl.Kd[0], currentTriMtl.Kd[1], currentTriMtl.Kd[2]);
			currentMaterial.specular = cyVec3f(currentTriMtl.Ks[0], currentTriMtl.Ks[1], currentTriMtl.Ks[2]);

	        if (currentTriMtl.map_Ka) 
	        {	
	        	// Exception for Render to Texture
	        	if (std::string(currentTriMtl.map_Ka.data) == "R2T")
	        	{
	        		// Bind some var in glState to texture?
	        		if (renderBuffer)
	        		{
	        			currentMaterial.texAmbient = renderBuffer->textureID;

	        			// Temporary hard coding for project 6 mirror
	        			// currentMaterial.sampleMirror = 1;
	        		}        		
	        	}
	        	else
	        	{
		    		currentMaterial.LoadTexture(processedPath + "/" + std::string(currentTriMtl.map_Ka.data),
		        							&currentMaterial.texAmbient);
		    	}
	        }

	        if (currentTriMtl.map_Kd) 
	        {
	        	if (std::string(currentTriMtl.map_Kd.data) == "R2T")
	        	{
	        		// Bind some var in glState to texture?
	        		if (renderBuffer)
	        		{
	        			currentMaterial.texDiffuse = renderBuffer->textureID;
	        		}        		
	        	}
	        	else
	        	{
	        		currentMaterial.LoadTexture(processedPath + "/" + std::string(currentTriMtl.map_Kd.data),
	            							&currentMaterial.texDiffuse); 
	        	}
	        }

	        if (currentTriMtl.map_Ks) 
	        {
	            currentMaterial.LoadTexture(processedPath + "/" + std::string(currentTriMtl.map_Ks.data),
	            							&currentMaterial.texSpecular);
	        }

	        // Using map_Ns as normal map in this case
	        if (currentTriMtl.map_Ns)
	        {
	        	currentMaterial.LoadTexture(processedPath + "/" + std::string(currentTriMtl.map_Ns.data),
	            							&currentMaterial.texNormal);
	        }

	        if (currentTriMtl.map_disp)
	        {
	        	currentMaterial.LoadTexture(processedPath + "/" + std::string(currentTriMtl.map_disp.data),
	            							&currentMaterial.texDisplacement);
	        }

	        materials.push_back(currentMaterial);
	        vertexCount += currentMaterial.numberOfVertices;
	    }    
    }
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
	glEnableVertexAttribArray(glStates->modelUV);

	// glDrawArrays(GL_TRIANGLES, 0, drawSize);
	// Draw the triangles under each material
	for (int i = 0; i < materials.size(); ++i)
	{
		materials[i].sendTo(glStates);

		// glDrawArrays(GL_TRIANGLES, materials[i].startIndex, materials[i].numberOfVertices);
		glDrawArrays(GL_PATCHES, materials[i].startIndex, materials[i].numberOfVertices);
		// glDrawArrays(GL_PATCHES, materials[i].startIndex, 4);
	}


	glDisableVertexAttribArray(glStates->modelPos);
	glDisableVertexAttribArray(glStates->modelNor);
	glDisableVertexAttribArray(glStates->modelUV);
}

void GLMesh::Center()
{
	auto modelCenter = mesh.GetBoundMin() + (mesh.GetBoundMax() - mesh.GetBoundMin())/2;

	// printf("Model Center: %f %f %f\n", modelCenter[0], modelCenter[1], modelCenter[2]);

	modelMatrix = cyMatrix4f::Translation(-modelCenter) * modelMatrix;
}

#endif // GL_MESH_H

