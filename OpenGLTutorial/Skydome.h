#pragma once
#include <vector>
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"

class Skydome
{
	friend class ModelRenderer;
public:
	Skydome(Texture * texture)
	{
		this->texture = texture;
	}
	virtual ~Skydome(){}

	void init()
	{
		float azimuth;
		int k;

		vector<glm::vec3> vertices;
		vector<glm::vec2> uv;
		vector<GLuint> indices;

		int resolution = 10;
		const float PI = 3.14159265;

		float spherePercentage = 2.0f;
		float radius = 1000.0f;
		float texturePercentage = 0.9f;

		const float azimuth_step = (PI * 2.0f) / resolution;
		const float elevation_step = spherePercentage * (PI/2) / (float)resolution;

		glm::vec3 vtx;
		glm::vec2 uvx;
		const float tcV = texturePercentage / resolution;

		for (k = 0, azimuth = 0; k <= resolution; k++)
		{
			float elevation = PI / 2;
			const float tcU = (float)k / (float)resolution;
			const float sinA = sinf(azimuth);
			const float cosA = cosf(azimuth);
			for (int j = 0; j <= resolution; j++)
			{
				const float cosEr = radius * cosf(elevation);
				vtx = glm::vec3(cosEr*sinA, radius*sinf(elevation), cosEr*cosA);
				uvx = glm::vec2(tcU, j*tcV);
				vertices.push_back(vtx);
				uv.push_back(uvx);
				elevation -= elevation_step;
			}
			azimuth += azimuth_step;
		}

		for (k = 0; k < resolution; k++)
		{
			indices.push_back(resolution + 2 + (resolution + 1) * k);
			indices.push_back(1 + (resolution + 1) * k);
			indices.push_back(0 + (resolution + 1) * k);

			for (int j = 1; j < resolution; j++)
			{
				indices.push_back(resolution + 2 + (resolution + 1) * k + j);
				indices.push_back(1 + (resolution + 1) * k + j);
				indices.push_back(0 + (resolution + 1) * k + j);

				indices.push_back(resolution + 1 + (resolution + 1) * k + j);
				indices.push_back(resolution + 2 + (resolution + 1) * k + j);
				indices.push_back(0 + (resolution + 1) * k + j);
			}
		}

		std::vector<GLfloat> vBuffer;
		for (int i = 0; i < vertices.size(); i++)
		{
			vBuffer.push_back(vertices[i].x);
			vBuffer.push_back(vertices[i].y);
			vBuffer.push_back(vertices[i].z);
		}

		std::vector<GLfloat> uvBuffer;
		for (int i = 0; i < uv.size(); i++)
		{
			uvBuffer.push_back(uv[i].x);
			uvBuffer.push_back(uv[i].y);
		}

		indexCount = indices.size();

		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), &vBuffer[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
		glBufferData(GL_ARRAY_BUFFER, uvBuffer.size() * sizeof(GLfloat), &uvBuffer[0], GL_STATIC_DRAW);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), &indices[0], GL_STATIC_DRAW);
	}

private:

	GLuint vertexBufferObject;
	GLuint uvBufferObject;
	GLuint indexBufferObject;

	Texture * texture;

	__int32 indexCount;
};