#pragma once
#include "Mapping.h"
#include "Code_Files/Trainer.h"
class Controller
{
	int mouse_x, mouse_y;
	SDL_Event e;
	Mapping* main_map;
	Trainer* player;

	//bool and ints to determine if the player can move again
	//move delay is in milliseconds
	bool can_move;
	int ticker;
	int move_delay;

	std::vector<char> function_queue;

public:
	Controller();
	Controller(SDL_Renderer* r);	
	~Controller();
	//setup to test battles before json implimentation
	void test_setup();

	//main controls for inputs
	void main_game_controller(SDL_Renderer* r);
	//menu controls
	void menu_controls();
	//controls for battles
	void battle_controls(SDL_Renderer* r);

	//bools to chek which control scheme is being used
	bool running;
	bool menuing;
};

