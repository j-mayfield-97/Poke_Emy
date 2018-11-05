#pragma once
//#include "Mapping.h"
#include "Code_Files/Trainer.h"
#include "Controller.h"

//GameMan will be the main containg class that will keep the controlls, maps , character movemnt , and logics.
class GameMan
{
public:
	GameMan();
	GameMan(SDL_Renderer * r);
	~GameMan();

	//Most important objects to run a game
	//Mapping* map;  I'd rather have Map as a member of GameMan, but for now controls needs access to it.
	//actually lster I could just pass the map as a parameter of controls
	Trainer* player;
	Controller* control;

	//update everything 
	void update(SDL_Renderer* r);

};

