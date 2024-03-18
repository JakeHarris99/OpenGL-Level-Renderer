#pragma once

#include <vector>
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"

using namespace std;

class Terrain
{
	friend class ModelRenderer;
public:
	Terrain(string heightMapFile, Texture * texture)
	{
		this->heightMapFile = heightMapFile;
		this->texture = texture;
	}
	virtual ~Terrain() {}

	void init()
	{
		SDL_Surface * surface = IMG_Load(this->heightMapFile.c_str());
		int bytesPerPixel = surface->format->BytesPerPixel;
		Uint32 * pixels = (Uint32*)surface->pixels;
		const int terrainHeight = surface->h - 1;
		const int terrainWidth = surface->w - 1;
		const int totalTiles = terrainWidth * terrainHeight;
		const int vertexCountX = terrainWidth + 1;
		const int vertexCountY = terrainHeight + 1;
		const int totalVertices = vertexCountX * vertexCountY;
		const int triangleCount = totalTiles * 2;
		const int indexCount = triangleCount * 3;
		this->indexCount = indexCount;
		glm::vec3 * vertices = new glm::vec3[totalVertices];
		glm::vec2 * uvCoords = new glm::vec2[totalVertices];
		GLuint * indices = new GLuint[indexCount];
		const int TILE_SIZE = 4;

		for(int i = 0; i < vertexCountX; i++)
		{
			for (int j = 0; j < vertexCountY; j++)
			{
				glm::vec2 uv;
				__int32 tileIdX = i;
				__int32 tileIdY = terrainHeight - j;
				uv.x = (float)tileIdX / terrainWidth;
				uv.y = (float)tileIdY / terrainHeight;
				__int32 tileHeight = (pixels[i * vertexCountX + j] >> 8) & 0xFF;
				vertices[i * vertexCountX + j] = glm::vec3(j * TILE_SIZE, (float)tileHeight*0.2, i * TILE_SIZE);
				uvCoords[i * vertexCountX + j] = uv;
			}
		}
		
		for (int i = 0; i < terrainWidth; i++)
		{
			for (int j = 0; j < terrainHeight; j++)
			{
				__int32 squareIndex = i * terrainWidth + j;
				__int32 triangleOffset = squareIndex * 6;

				indices[triangleOffset + 0] = i * vertexCountX + j + 0;
				indices[triangleOffset + 2] = i * vertexCountX + j + vertexCountX + 1;
				indices[triangleOffset + 1] = i * vertexCountX + j + vertexCountX + 0;

				indices[triangleOffset + 3] = i * vertexCountX + j + 0;
				indices[triangleOffset + 5] = i * vertexCountX + j + 1;
				indices[triangleOffset + 4] = i * vertexCountX + j + vertexCountX + 1;
			}
		}

		std::vector<GLfloat> vBuffer;
		for (int i = 0; i < totalVertices; i++)
		{
			vBuffer.push_back(vertices[i].x);
			vBuffer.push_back(vertices[i].y);
			vBuffer.push_back(vertices[i].z);
		}
		std::vector<GLfloat> uvBuffer;
		for (int i = 0; i < totalVertices; i++)
		{
			uvBuffer.push_back(uvCoords[i].x);
			uvBuffer.push_back(uvCoords[i].y);
		}
		std::vector<GLuint> indexBuffer;
		for (int i = 0; i < indexCount; i++)
		{
			indexBuffer.push_back(indices[i]);
		}

		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), &vBuffer[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
		glBufferData(GL_ARRAY_BUFFER, uvBuffer.size() * sizeof(GLfloat), &uvBuffer[0], GL_STATIC_DRAW);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, indexBuffer.size() * sizeof(GLfloat), &indexBuffer[0], GL_STATIC_DRAW);
	}

private:
	char * heightData;
	string heightMapFile;

	GLuint vertexBufferObject;
	GLuint uvBufferObject;
	GLuint indexBufferObject;

	Texture * texture;

	__int32 indexCount;
};