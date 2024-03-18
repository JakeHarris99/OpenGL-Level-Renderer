#pragma once
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include "ModelRenderer.h"
#include "FPSCamera.h"
#include "Model.h"
#include "GUIRenderer.h"

class GameLoop {

public:
	GameLoop();
	virtual ~GameLoop();

	void init();
	bool handleInput();
	void update();
	void draw();
	void clean();

private:
	SDL_Window * window;
	SDL_GLContext glContext;

	ModelRenderer * modelRenderer;
	GUIRenderer * guiRenderer;

	FPSCamera * camera;

	Model * rockOne;
	Model * rockTwo;
	Model * rockThree;
	Model * rockFour;
	Model * satellite;
	Texture * rockTexture;
	Texture * satelliteTexture;

	Terrain * terrain;
	Texture * terrainTexture;
	
	Texture * skyTexture;
	Skydome * skydome;

	Billboard * billboard;

	Font * font;
	GUIText * dataText;

	time_t start;

	int frames;
	double starttime;
	bool first;
	float fps;
};