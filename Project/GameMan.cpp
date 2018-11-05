#include "GameMan.h"

GameMan::GameMan()
{
}

GameMan::GameMan(SDL_Renderer* r)
{
	//map = new Mapping();
	player = new Trainer();
	control = new Controller(r);
}


GameMan::~GameMan()
{
	control->~Controller();
	//map->~Mapping();
	player->~Trainer();
}

void GameMan::update(SDL_Renderer* r)
{
	control->main_game_controller(r);
	//update trainer logic
}
