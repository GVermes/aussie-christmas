#include "Animation.h"

Animation::Animation(SDL_Texture * spriteSheet, SDL_Renderer * renderer, int numberOfFrames, int frameWidth, int frameHeight, float frameDuration)
{
	this->spriteSheet = spriteSheet;
	this->renderer = renderer;
	this->numberOfFrames = numberOfFrames;
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	this->frameDuration = frameDuration;

	currentFrame = 0;
	frameTimer = 0;
}

void Animation::update(float dt)
{
	
	frameTimer += dt; 

	if (frameTimer >= frameDuration)
	{
		currentFrame++;
		frameTimer = 0;

		if (currentFrame >= numberOfFrames) {
			currentFrame = 0;
		}
	}
}

void Animation::draw(int x, int y)
{
	//get current clipping region
	SDL_Rect clip;
	clip.x = currentFrame * frameWidth;
	clip.y = 0;
	clip.w = frameWidth;
	clip.h = frameHeight;
	//setup destination region
	SDL_Rect dest = { x, y, frameWidth, frameHeight };
	//draw
	SDL_RenderCopy(renderer, spriteSheet, &clip, &dest);
}

void Animation::draw(int x, int y, float scale)
{
	//get current clipping region
	SDL_Rect clip;
	clip.x = currentFrame * frameWidth;
	clip.y = 0;
	clip.w = frameWidth;
	clip.h = frameHeight;
	//setup destination region
	SDL_Rect dest = { x, y, frameWidth*scale, frameHeight*scale };
	//draw
	SDL_RenderCopy(renderer, spriteSheet, &clip, &dest);
}

void Animation::draw(int x, int y, bool flip)
{
	//get current clipping region
	SDL_Rect clip;
	clip.x = currentFrame * frameWidth;
	clip.y = 0;
	clip.w = frameWidth;
	clip.h = frameHeight;
	//setup destination region
	SDL_Rect dest = { x, y, frameWidth, frameHeight };
	//draw
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	if (flip)
		flipType = SDL_FLIP_HORIZONTAL;
	
	SDL_RenderCopyEx(renderer, spriteSheet, &clip, &dest, 0, NULL, flipType);
}
