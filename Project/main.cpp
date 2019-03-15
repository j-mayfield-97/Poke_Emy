#pragma once
//Jason Mayfield

#include <iostream>
#include "SDL.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameMan.h"
#include "JSONTEST.h"
/*
TODO

battle menu ui
set up mons/moves/effects with the json parser
change all the console messages into SDL text ingame
other trainers
other trainers ai
items and item shops
restructure code to be more elegant
go through all your notes /clean up old notes and comments
time-stamp mons so they have unique id's
set up what a turn is
error handling - json schemas and validation
random int / timestamped ID for identifiying mons, instead of using name strings .. possibly uuid from lua
std::set might help runtime instead of using std::vector
keep a list of all tags that have been used in order to make unique tags
clean 
make vectors ordered list to speed up searches

FIXED
fix map obstacle logic
get battles functional - battle queue
make the games Room based instead of infinite directions
large strange printing of lines to the console
YYYYYYYYYYYYYY in the Name element of Mons


PRESSING BUGS


Ideas

crits or no crits?
you can go in either direction , as long as you continue the game will get harder
Hearthstoneish tag types
run based 
room by room based
monster collecting eit nuzlockish rules -have to name the mons -only catch one every room
optional room bosses and forced room bosses
item shops
an item to redo the room you've just completed 
a move that kill all your current mons for one of the opponents mons 
a move that uses half your hp to block an status effects and damage for a turn
move where one mon holds another mon making it increasing damage
unlock mon for new runs permenantly
types can be based on emotions?
tags instead of types
like a five tag limit
smile heals everyone around you
grin frightens opponent (wont be targeted this turn)
HMs useable in rooms
puzzle rooms???
no healing in between battles /unless bought or found item
types could be reduced to sharp, blunt, heal
*/

SDL_Renderer* ren;
SDL_Window* win;
GameMan* gam;


//maps tiles of 50px will be 50 by 35x30

void init()
{
	//make the window
	win = SDL_CreateWindow("MONs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

	if (win == NULL)
	{
		std::cout << "window creation failed" << std::endl;
	}
	else
	{
		//make the renderer
		ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

		if (ren == NULL)
		{
			std::cout << "renderer creation failed" << std::endl;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) &imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			}

			//iconMake(win);//icon not created yet

			//mouse cursor vanisher
			SDL_ShowCursor(SDL_DISABLE);
			//SDL_SetRelativeMouseMode(SDL_TRUE); // this should lock the mouse in place

		}
	}
	//start audio capabilities
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	//open the font
	TTF_Init();
	//mainFont = TTF_OpenFont("assets/earwig.ttf", 30);
}


void fps_60()//keep the game running at a smooth 60fps
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	frameStart = SDL_GetTicks();

	frameTime = SDL_GetTicks() - frameStart;

	if (frameDelay > frameTime)
	{
		SDL_Delay(frameDelay - frameTime); //get ttf to print fps at the top
	}
}

//clean to be run on exit loop
void clean()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	delete gam;
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

void game_loop()
{
	init();
	//instantiate
	gam = new GameMan(ren);

	while (gam->control->running)
	{
		fps_60();
		//draw map
		//draw character
		//for now controls is holding the majority of the objects related to the game
		gam->control->main_game_controller(ren);
		//render to window
		SDL_RenderPresent(ren);
		//clear renderer
		SDL_RenderClear(ren);
	}
	clean();
}

int main(int argc, char * argv[])
{
	game_loop();
	return 0;
}
