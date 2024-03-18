#include "GameLoop.h"
#include <time.h>

GameLoop::GameLoop()
{
	camera = new FPSCamera(this->window);
	modelRenderer = new ModelRenderer(camera);
	guiRenderer = new GUIRenderer();
	rockOne = new Model("Rock.obj", glm::vec3(50,0,50), glm::vec3(0,0,0), glm::vec3(0.2f,0.2f,0.2f), false);
	rockTwo = new Model("Rock2.obj", glm::vec3(-50, 0, -50), glm::vec3(0, 0, 0), glm::vec3(10.0f, 10.0f, 10.0f), false);
	rockThree = new Model("Rock3.obj", glm::vec3(-50, 0, 50), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f),false);
	rockFour = new Model("Rock4.obj", glm::vec3(100, 0, -100), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f),false);
	satellite = new Model("satellite.obj", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f),false);
	rockTexture = new Texture("rock.jpg");
	terrainTexture = new Texture("terrain-texture.png");
	satelliteTexture = new Texture("satellite-texture.jpg");
	terrain = new Terrain("terrain-heightmap.png", terrainTexture);
	skyTexture = new Texture("skydome.jpg");
	skydome = new Skydome(skyTexture);
	billboard = new Billboard(rockTexture);
	font = new Font("font.png");
	dataText = new GUIText(font);
	start = time(0);
	frames = 0;
	starttime = 0;
	first = true;
	fps = 0.0f;
}

GameLoop::~GameLoop()
{
	delete camera;
	delete modelRenderer;
	delete terrainTexture;
	delete rockOne;
	delete rockTwo;
	delete rockThree;
	delete rockFour;
	delete satellite;
	delete rockTexture;
	delete satelliteTexture;
	delete terrain;
	delete skyTexture;
	delete skydome;
	delete billboard;
	delete font;
	delete guiRenderer;
	delete dataText;
}

void GameLoop::init()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("SDL OpenGL", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		printf("SDL could not create window! SDL Error: %s\n", SDL_GetError());
		return;
	}

	glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr) {
		printf("SDL could not create GL context! SDL Error: %s\n", SDL_GetError());
		return;
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	modelRenderer->init();
	guiRenderer->init();
	rockOne->init();
	rockTwo->init();
	rockThree->init();
	rockFour->init();
	satellite->init();
	rockTexture->init();
	satelliteTexture->init();
	rockOne->setTexture(rockTexture);
	rockTwo->setTexture(rockTexture);
	rockThree->setTexture(rockTexture);
	rockFour->setTexture(rockTexture);
	satellite->setTexture(satelliteTexture);
	terrainTexture->init();
	terrain->init();
	skyTexture->init();
	skydome->init();
	billboard->init();
	font->init();
}

bool GameLoop::handleInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		camera->handleInput(e);
	}
	return true;
}

void GameLoop::update()
{
	satellite->update();
	camera->update();
	double timepassed = difftime(time(0), start);
	if (first)
	{
		frames = 0;
		starttime = timepassed;
		first = false;
	}
	frames++;
	if (timepassed - starttime > 0.25 && frames > 10)
	{
		fps = (double)frames / (timepassed - starttime);
		if (!camera->UItoggle)
		{
			dataText->setText("Position: X: " + to_string((int)camera->getPosition().x) + ", Z: " + to_string((int)camera->getPosition().z));
		}
		else
		{
			dataText->setText("fps: " + to_string((int)fps));
		}
		starttime = timepassed;
		frames = 0;
	}
}

void GameLoop::draw()
{

	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelRenderer->renderTerrain(terrain);
	modelRenderer->renderModel(rockOne);
	modelRenderer->renderModel(rockTwo);
	modelRenderer->renderModel(rockThree);
	modelRenderer->renderModel(rockFour);
	modelRenderer->renderModel(satellite);
	modelRenderer->renderSkydome(skydome);
	guiRenderer->renderText(dataText, glm::vec3(0,0,0));
	//modelRenderer->renderBillboard(billboard);
	SDL_GL_SwapWindow(window);
}

void GameLoop::clean()
{

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

#undef main

int main(void) {
	GameLoop * gameLoop = new GameLoop();
	gameLoop->init();

	while (gameLoop->handleInput()) {
		gameLoop->update();
		gameLoop->draw();
	}

	gameLoop->clean();
	delete gameLoop;
}
