#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "../Enum_Pokes.h"
#include <vector>
//FOR SOME REASON This has access to main's renderer - ren
//get all logic of menu into this class
struct BattleStruct
{
	BattleStruct();
	BattleStruct(SDL_Renderer *r);

	void texture_wipe();
	void ren_copies(SDL_Renderer* r);
	void hovering(SDL_Renderer* r);

	SDL_Rect* closest_left();

	SDL_Rect*  closest_right();

	SDL_Rect* closest_up();

	SDL_Rect* closest_down();

	SDL_Rect battlingRect ;
	SDL_Rect difficulty ; //on and off button style load easy and hard
	SDL_Rect opt ; // opt cahnges to opt window where you pick bot or player side by side
	SDL_Rect player ;
	SDL_Rect bot ;
	SDL_Rect quit ;
	SDL_Rect* currentRect ;

private:
	SDL_Texture* menutx;
	SDL_Texture* overtx ;
	SDL_Texture* hovertx;
	SDL_Texture* difftx;
	SDL_Texture* opttx;
	SDL_Texture* playertx;
	SDL_Texture* bottx;
	SDL_Texture* quittx;

	//array for iterating through menu tiles

	std::vector<SDL_Rect*> rect_pntr_vec;

//for swaping easy and hard tiles
	int EHswapCheck = 1;
};

BattleStruct::BattleStruct() 
{ 
}

BattleStruct::BattleStruct(SDL_Renderer* r)
{
	//init rects
	 battlingRect = SDL_Rect();
	 difficulty = SDL_Rect(); //on and off button style load easy and hard
	 opt = SDL_Rect(); // opt cahnges to opt window where you pick bot or player side by side
	 player = SDL_Rect();
	 bot = SDL_Rect();
	 quit = SDL_Rect();
	currentRect = &SDL_Rect();

	//Menu Sign at top
	battlingRect.h = screenHeight / 4;
	battlingRect.w = screenWidth / 4;
	battlingRect.x = 0;
	battlingRect.y = 0;

	//option and player are not operational 
	bot.h = battlingRect.h;
	bot.w = battlingRect.w;
	bot.x = 0;
	bot.y = 0;

	opt.h = battlingRect.h;
	opt.w = battlingRect.w;
	opt.x = bot.x + bot.w;
	opt.y = bot.y + bot.h;

	player.h = battlingRect.h;
	player.w = battlingRect.w;
	player.x = opt.x + opt.w;
	player.y = opt.y + opt.h;

	difficulty.h = battlingRect.h;
	difficulty.w = battlingRect.w;
	difficulty.x = screenWidth / 4;
	difficulty.y = opt.h + opt.y + 5;

	quit.h = battlingRect.h;
	quit.w = battlingRect.w;
	quit.x = player.x + player.w;
	quit.y = player.y + player.h;

	//current rect starts at bot
	currentRect = &bot;

	menutx = IMG_LoadTexture(r, "assets/menu.png");
	overtx = IMG_LoadTexture(r, "assets/redOverlay.png");
	hovertx = IMG_LoadTexture(r, "assets/blueOverlay.png");
	opttx = IMG_LoadTexture(r, "assets/menuOpt.png");
	difftx = IMG_LoadTexture(r, "assets/menuEasy.png");// i technically dont have to load it here , if i run the ehswap(), would it be better to not do unnecessary loads or not do unnecessary calls
	playertx = IMG_LoadTexture(r, "assets/prikupine.png");
	bottx = IMG_LoadTexture(r, "assets/menuBot.png");
	quittx  = IMG_LoadTexture(r, "assets/menuQuit.png");

	//array for iterating through menu tiles
	//these are the only rects linked to a functionality in controls.cpp
	rect_pntr_vec.push_back(&player);
	rect_pntr_vec.push_back(&opt);
	rect_pntr_vec.push_back(&bot);
	rect_pntr_vec.push_back(&quit);
}

//clean the textures
void BattleStruct::texture_wipe()
{
	SDL_DestroyTexture(menutx);
	SDL_DestroyTexture(overtx);
	SDL_DestroyTexture(hovertx);
	SDL_DestroyTexture(difftx);
	SDL_DestroyTexture(opttx);
	SDL_DestroyTexture(playertx);
	SDL_DestroyTexture(bottx);
	SDL_DestroyTexture(quittx);
}

//this has no use currently
/*
void BattleStruct::EHswap(SDL_Renderer* r) 
{
	EHswapCheck *= -1;// keeps track of difficulty setting
	SDL_DestroyTexture(difftx);//prevent the leaks
	if (EHswapCheck < 0){ difftx = IMG_LoadTexture(r, "assets/menuHard.png"); }
	else{ difftx = IMG_LoadTexture(r, "assets/menuEasy.png"); }
}*/

void BattleStruct::hovering(SDL_Renderer * r) // maybe just pass a rect adress instead to set current rect
{
	SDL_RenderCopy(r, hovertx, NULL, currentRect);
}

void BattleStruct::ren_copies(SDL_Renderer* r)
{

	hovering(r);

	//SDL_RenderCopy(r, overtx, NULL, &battlingRect);
	SDL_RenderCopy(r, overtx, NULL, &opt);
	SDL_RenderCopy(r, overtx, NULL, &player);
	SDL_RenderCopy(r, overtx, NULL, &bot);
	//SDL_RenderCopy(r, overtx, NULL, &difficulty);
	SDL_RenderCopy(r, overtx, NULL, &quit);

	//SDL_RenderCopy(r, menutx, NULL, &battlingRect);
	SDL_RenderCopy(r, opttx, NULL, &opt);
	SDL_RenderCopy(r, playertx, NULL, &player);
	SDL_RenderCopy(r, bottx, NULL, &bot);
	//SDL_RenderCopy(r, difftx, NULL, &difficulty); //make it run on swaptx
	SDL_RenderCopy(r, quittx, NULL, &quit);
}

///these could prob be written more elegantly later
SDL_Rect* BattleStruct::closest_left()
{
	//init a rect pointer which will hold the address of the desired rect
	SDL_Rect* nearest = new SDL_Rect;
	nearest->x = 0;
	nearest->y = 0;
	nearest->w = 0;
	nearest->h = 0;

	//distances to compare and find nearest rect
	int current_dist = 0;
	int closest_dist = screenWidth;

	//store the current rect 
	SDL_Rect* store = currentRect;


	//iterate through the array if the x value is small then compare it against the nearest's distance from current
	for (SDL_Rect* i : rect_pntr_vec)
	{
		//if i.x is less than curr x then its left 
		if (i->x < currentRect->x)
		{
			current_dist = abs(currentRect->x - i->x);

			//keep the closest distance and the closest rectangle
			if (current_dist < closest_dist)
			{
				closest_dist = current_dist;
				nearest = i;
			}
		}
	}
	//reject anything outside of the screen or of unresonable portions
	if (nearest->w <= 0  || nearest->w >= screenWidth || 
		nearest->h <= 0 || nearest->h >= screenHeight || 
		nearest->x < 0 || nearest->x >= screenWidth ||
		nearest->y < 0 || nearest->y >= screenHeight)
			return store;
	else
		return nearest;
}

SDL_Rect* BattleStruct::closest_right()
{
	//store the current nearest
	SDL_Rect* nearest = new SDL_Rect;
	nearest->x = 0;
	nearest->y = 0;
	nearest->w = 0;
	nearest->h = 0;

	int dist1 = 0;
	int dist2 = screenWidth;

	//store the current rect to return if outcomes are un sensible
	SDL_Rect* store = currentRect;

	//iterate through the array if the x value is small then compare it against the nearest's distance from current
	for (SDL_Rect* i : rect_pntr_vec)
	{
		//if i.x is less than curr x then its left 
		if (i->x > currentRect->x)
		{
			dist1 = abs(currentRect->x - i->x);

			//set to the closest distance
			if (dist1 < dist2)
			{
				dist2 = dist1;
				nearest = i;
			}
		}
	}
	//reject anything outside of the screen or of unresonable portions
	if (nearest->w <= 0 || nearest->w >= screenWidth ||
		nearest->h <= 0 || nearest->h >= screenHeight ||
		nearest->x < 0 || nearest->x >= screenWidth ||
		nearest->y < 0 || nearest->y >= screenHeight)
		return store;
	else
		return nearest;
}

SDL_Rect* BattleStruct::closest_up()
{
	//init a rect pointer which will hold the address of the desired rect
	SDL_Rect* nearest = new SDL_Rect;
	nearest->x = 0;
	nearest->y = 0;
	nearest->w = 0;
	nearest->h = 0;

	//distances to compare and find nearest rect
	int current_dist = 0;
	int closest_dist = screenWidth;

	//store the current rect 
	SDL_Rect* store = currentRect;


	//iterate through the array if the x value is small then compare it against the nearest's distance from current
	for (SDL_Rect* i : rect_pntr_vec)
	{
		//if i.x is less than curr x then its left 
		if (i->y < currentRect->y)
		{
			current_dist = abs(currentRect->x - i->x);

			//keep the closest distance and the closest rectangle
			if (current_dist < closest_dist)
			{
				closest_dist = current_dist;
				nearest = i;
			}
		}
	}
	//reject anything outside of the screen or of unresonable portions
	if (nearest->w <= 0 || nearest->w >= screenWidth ||
		nearest->h <= 0 || nearest->h >= screenHeight ||
		nearest->x < 0 || nearest->x >= screenWidth ||
		nearest->y < 0 || nearest->y >= screenHeight)
		return store;
	else
		return nearest;
}

SDL_Rect* BattleStruct::closest_down()
{
	//store the current nearest
	SDL_Rect* nearest = new SDL_Rect;
	nearest->x = 0;
	nearest->y = 0;
	nearest->w = 0;
	nearest->h = 0;

	int dist1 = 0;
	int dist2 = screenWidth;

	//store the current rect to return if outcomes are un sensible
	SDL_Rect* store = currentRect;

	//iterate through the array if the x value is small then compare it against the nearest's distance from current
	for (SDL_Rect* i : rect_pntr_vec)
	{
		//if i.x is less than curr x then its left 
		if (i->y > currentRect->y)
		{
			dist1 = abs(currentRect->y - i->y);

			//set to the closest distance
			if (dist1 < dist2)
			{
				dist2 = dist1;
				nearest = i;
			}
		}
	}
	//reject anything outside of the screen or of unresonable portions
	if (nearest->w <= 0 || nearest->w >= screenWidth ||
		nearest->h <= 0 || nearest->h >= screenHeight ||
		nearest->x < 0 || nearest->x >= screenWidth ||
		nearest->y < 0 || nearest->y >= screenHeight)
		return store;
	else
		return nearest;
}