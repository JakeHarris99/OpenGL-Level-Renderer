#pragma once

#include <glew.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

#include "GUIText.h"

class GUIRenderer
{
public:
	GUIRenderer();
	virtual ~GUIRenderer();

	void init();
	void renderText(GUIText * text, glm::vec3 textPosition);

private:
	GLuint programId = 0;

	GLuint vertexPositionLocation = -1;
	GLuint uvLocation = -1;

	GLuint translationLocation = -1;
	GLuint textureSamplerLocation = -1;
};