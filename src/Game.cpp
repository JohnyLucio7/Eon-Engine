#include <iostream>
#include "./Constants.h"
#include "./Game.h"

Game::Game(){
	this->isRunning = false;
}

Game::~Game(){
}

bool Game::IsRunning() const{
	return this->isRunning;
}

float projectilePosX = 0.0f;
float projectilePosY = 0.0f;
float projectileVelX = 20.0f;
float projectileVelY = 20.0f;

void Game::Initialize(int width, int height){
	
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}

	this->window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_BORDERLESS
	);

	if(!this->window){
		std::cerr << "Error Creating SDL window" << std::endl;
		return;
	}

	this->renderer = SDL_CreateRenderer(window, -1, 0);

	if(!this->renderer){
		std::cerr << "Error Creating SDL renderer" << std::endl;
		return;
	}

	this->isRunning = true;
	return;
}

void Game::ProcessInput(){
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type){
		case SDL_QUIT:
			this->isRunning = false;
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE){
				this->isRunning = false;
			}
			break;
		default:
			break;
	}
}


void Game::Update(){
	
	// Sleep the executation until we reach the target frame time in milliseconds
	int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

	// Only call delay if we are too fast too process this frame 
	if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME){
		SDL_Delay(timeToWait);
	}

	// Delta time is the difference in ticks from last frame converted to seconds
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	// Clamp deltaTime to a maximum value
	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;
	
	// Set the new ticks for the current frame to be used in the next pass
	ticksLastFrame = SDL_GetTicks();

	projectilePosX += projectileVelX * deltaTime;
	projectilePosY += projectileVelY * deltaTime;
}

void Game::Render(){

	SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
	SDL_RenderClear(this->renderer);

	SDL_Rect projectile {
		(int) projectilePosX,
		(int) projectilePosY,
		10,
		10
	};

	SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(this->renderer, &projectile);

	SDL_RenderPresent(this->renderer);
}

void Game::Destroy(){
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}
