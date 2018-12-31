#include "GameMan.h"

GameMan::GameMan()
{
}

GameMan::GameMan(SDL_Renderer* r)
{
	//map = new Mapping(); Mapping is now an object in container
	//player = new Trainer();
	control = new Controller(r);
}


GameMan::~GameMan()
{
	control->~Controller();
	//map->~Mapping();
	//player->~Trainer();
}

void update(SDL_Renderer* r, Trainer* player)
{
	//control->main_game_controller(r);
	//update trainer logic
}
