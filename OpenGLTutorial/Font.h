#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Texture.h"

using namespace std;

class Font
{
	friend class GUIText;
	friend class ModelRenderer;
	friend class GUIRenderer;
public:
	Font(string fontTexture)
	{
		this->fontTexture = fontTexture;
		this->texture = new Texture(fontTexture);
	}

	virtual ~Font()
	{
		delete texture;
	}

	void init()
	{
		SDL_Surface * surface = IMG_Load(this->fontTexture.c_str());
		pixelData = (Uint32*)surface->pixels;
		imageW = surface->w;
		imageH = surface->h;
		int pixelCount = imageW * imageH;
		int colorTopLeft = getPixel(0, 0);
		int colorLowerRight = getPixel(1, 0);
		int colorBackground = getPixel(2, 0);
		int colorBackgroundTransparent = 0;

		vector<glm::vec2> topLeftPositions;
		vector<glm::vec2> bottomRightPositions;

		glm::vec2 position;
		for (position.y = 0; position.y < imageH; position.y++)
		{
			for (position.x = 0; position.x < imageW; position.x++)
			{
				int y = position.y;
				int x = position.x;
				int color = getPixel(position.x, position.y);
				if (color == colorTopLeft)
				{
					pixelData[y * imageW + x] = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
					topLeftPositions.push_back(position);
				}
				if (color == colorLowerRight)
				{
					pixelData[y * imageW + x] = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
					bottomRightPositions.push_back(position);
				}
				if (color == colorBackground)
				{
					pixelData[y * imageW + x] = SDL_MapRGBA(surface->format, 0, 0, 0, 0);
				}
			}
		}
		characterSize = 0;
		for (int i = 0; i < topLeftPositions.size(); i++)
		{
			FontArea fa;
			fa.x = topLeftPositions[i].x / (float)imageW;
			fa.y = topLeftPositions[i].y / (float)imageH;
			fa.x2 = bottomRightPositions[i+1].x / (float)imageW;
			fa.y2 = bottomRightPositions[i+1].y / (float)imageH;
			fa.w = bottomRightPositions[i+1].x - topLeftPositions[i].x;

			if (fa.w > characterSize)
			{
				characterSize = fa.w;
			}

			fontAreas.push_back(fa);
		}
		this->texture->init(surface);
	}

private:
	struct FontArea
	{
		float x;
		float y;
		float x2;
		float y2;
		int w;
	};

	int characterSize;

	vector<FontArea> fontAreas;
	string fontTexture;
	Uint32 * pixelData;
	int imageW;
	int imageH;

	Texture * texture;

	int getPixel(int x, int y)
	{
		return pixelData[y * imageW + x];
	}
};