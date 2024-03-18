#pragma once

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

#include "FPSCamera.h"
#include "Model.h"
#include "Terrain.h"
#include "Skydome.h"
#include "Billboard.h"
#include "Font.h"
#include "GUIText.h"


class ModelRenderer {
public:
	ModelRenderer(FPSCamera * camera);
	virtual ~ModelRenderer();

	void init();
	void renderModel(Model * m);
	void renderTerrain(Terrain * t);
	void renderSkydome(Skydome * s);
	void renderBillboard(Billboard * b);
	void renderText(GUIText * t);

private:

	GLuint programId=0;
	GLint vertexPositionLocation =-1;
	GLint uvLocation = -1;

	GLuint modelMatrixLocation = -1;
	GLuint viewMatrixLocation = -1;
	GLuint projectionMatrixLocation = -1;
	GLuint textureSamplerLocation = -1;

	FPSCamera * camera;
};