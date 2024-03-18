#pragma once

#include <sdl.h>
#include <glm\glm.hpp>
#include "Camera.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define I 4

class FPSCamera : public Camera
{
	friend class ModelRenderer;
public:
	FPSCamera(SDL_Window * window)
	{
		this->window = window;
		SDL_GetMouseState(&oldMouseX, &oldMouseY);
	}
	virtual ~FPSCamera() {}

	bool UItoggle = false;

	void handleInput(SDL_Event &e)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_UP:
				keyStates[UP] = true;
				break;
			case SDL_SCANCODE_DOWN:
				keyStates[DOWN] = true;
				break; 
			case SDL_SCANCODE_LEFT:
				keyStates[LEFT] = true;
				break;
			case SDL_SCANCODE_RIGHT:
				keyStates[RIGHT] = true;
				break;
			case SDL_SCANCODE_I:
				keyStates[I] = true;
				break;
			}
		}
		if (e.type == SDL_KEYUP)
		{
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_UP:
				keyStates[UP] = false;
				break;
			case SDL_SCANCODE_DOWN:
				keyStates[DOWN] = false;
				break;
			case SDL_SCANCODE_LEFT:
				keyStates[LEFT] = false;
				break;
			case SDL_SCANCODE_RIGHT:
				keyStates[RIGHT] = false;
				break;
			case SDL_SCANCODE_I:
				keyStates[I] = false;
				break;
			}
		}
	}

	void update()
	{

		if (keyStates[UP])
		{
			position.x += sin(glm::radians(yaw)) * movementSpeed;
			position.z -= cos(glm::radians(yaw)) * movementSpeed;
		}
		if (keyStates[DOWN])
		{
			position.x -= sin(glm::radians(yaw)) * movementSpeed;
			position.z += cos(glm::radians(yaw)) * movementSpeed;
		}
		if (keyStates[LEFT])
		{
			position.x -= cos(glm::radians(yaw)) * movementSpeed;
			position.z -= sin(glm::radians(yaw)) * movementSpeed;
		}
		if (keyStates[RIGHT])
		{
			position.x += cos(glm::radians(yaw)) * movementSpeed;
			position.z += sin(glm::radians(yaw)) * movementSpeed;
		}
		if (keyStates[I])
		{
			UItoggle = !UItoggle;
		}

		int x, y;
		SDL_GetMouseState(&x, &y);
		float diffX = (x - oldMouseX) * mouseSens;
		float diffY = (y - oldMouseY) * mouseSens;

		yaw += diffX;
		pitch += diffY;

		if (pitch > 40.0f)
		{
			pitch = 40.0f;
		}
		if (pitch < -40.0f)
		{
			pitch = -40.0f;
		}

		glm::mat4 matRoll = glm::mat4(1.0f);
		glm::mat4 matPitch = glm::mat4(1.0f);
		glm::mat4 matYaw = glm::mat4(1.0f);

		matRoll = glm::rotate(matRoll, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		matPitch = glm::rotate(matPitch, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		matYaw = glm::rotate(matYaw, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 rotate = matRoll * matPitch * matYaw;

		glm::mat4 translate = glm::mat4(1.0f);
		translate = glm::translate(translate, -position);
		viewMatrix = rotate * translate;

		SDL_WarpMouseInWindow(window, 320, 240);
		oldMouseX = 320;
		oldMouseY = 240;
	}

private:
	bool keyStates[5];
	float yaw = 0.0f, pitch = 0.0f;
	int oldMouseX, oldMouseY;
	float movementSpeed = 0.003f;
	float mouseSens = 0.1f;
	SDL_Window * window;
};