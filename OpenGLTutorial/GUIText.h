#pragma once

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>


#include "Font.h"

using namespace std;

class GUIText
{
	friend class ModelRenderer;
	friend class GUIRenderer;
public:
	GUIText(Font * font)
	{
		this->font = font;
	}

	virtual ~GUIText() {}

	void setText(string text)
	{
		vector<GLfloat> vertices;
		vector<GLfloat> uv;
		vector<GLuint> indices;

		float x = 0.0f;

		for (int i = 0; i < text.length(); i++)
		{
			int idx = text[i] - 32;
			float characterWidth = ((float)font->fontAreas[idx].w / (float)font->characterSize) * 0.1;

			vertices.push_back(x);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);

			vertices.push_back(x + characterWidth);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);

			vertices.push_back(x);
			vertices.push_back(-0.1f);
			vertices.push_back(0.0f);

			vertices.push_back(x + characterWidth);
			vertices.push_back(-0.1f);
			vertices.push_back(0.0f);

			x += characterWidth;

			indices.push_back((i * 4) + 0);
			indices.push_back((i * 4) + 1);
			indices.push_back((i * 4) + 2);
			indices.push_back((i * 4) + 2);
			indices.push_back((i * 4) + 1);
			indices.push_back((i * 4) + 3);

			uv.push_back(font->fontAreas[idx].x);
			uv.push_back(font->fontAreas[idx].y);
			uv.push_back(font->fontAreas[idx].x2);
			uv.push_back(font->fontAreas[idx].y);
			uv.push_back(font->fontAreas[idx].x);
			uv.push_back(font->fontAreas[idx].y2);
			uv.push_back(font->fontAreas[idx].x2);
			uv.push_back(font->fontAreas[idx].y2);

			glGenBuffers(1, &vertexBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &uvBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
			glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(GLfloat), &uv[0], GL_STATIC_DRAW);

			glGenBuffers(1, &indexBufferObject);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			indexCount = indices.size();
		}
	}
private:
	Font * font;

	GLuint vertexBufferObject = -1;
	GLuint uvBufferObject = -1;
	GLuint indexBufferObject = -1;

	__int32 indexCount;
};