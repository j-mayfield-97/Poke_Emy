#pragma once

#include "Controller.h"
#include "Code_Files/BattleMenu.h"
#include <SDL_image.h>
Controller::Controller()
{
}

Controller::Controller(SDL_Renderer* r)
{
	//init looping bools
	running = true;
	menuing = false;

	//movemment varaiables
	can_move = true;
	ticker = SDL_GetTicks();
	move_delay = 200;

	//instanciate the map
	main_map = new Mapping(r); 
	//make the first map
	main_map->Form_Initial_Map();
	//initial draw of the player
	main_map->animate_player_help(20, 32, 2, 2, 3, 0, 0, 10, move_delay);

	//instance player
	player = new Trainer();
}

Controller::~Controller()
{
}

void Controller::main_game_controller(SDL_Renderer* r)
{
	//input type for concurrent key presses
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT || currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			running = false;
		}
	}
	

	//run on left shift in all directions
	if (currentKeyStates[SDL_SCANCODE_LSHIFT] )
	{
		//affect move delay to change speeds on shift key
		move_delay = 150;
		if (currentKeyStates[SDL_SCANCODE_W] && can_move && !main_map->up_obsticle)
		{
			//upward run animation
			main_map->move_row_offscreen(true);
			main_map->animate_player_help(20, 32, 3, -1, 3, 1, 6, 10, move_delay);

			can_move = false;
		}
		if (currentKeyStates[SDL_SCANCODE_S] && can_move && !main_map->dwn_obsticle)
		{
			//downward run animation
			main_map->move_row_offscreen(false);
			main_map->animate_player_help(20, 32, 2, -1, 3, 1, 0, 10, move_delay);

			can_move = false;
		}
	}
	else
	{
		//walk up
		if (currentKeyStates[SDL_SCANCODE_W] && can_move && !main_map->up_obsticle)
		{
			//reset to standard delay
			move_delay = 300;
			//give the illsuion of movement by moving the map
			main_map->move_row_offscreen(true);
			//up facing animation
			main_map->animate_player_help(20, 32, 2, 2, 3, 0, 6, 10, move_delay);

			//prevent movemnt until the delay time has passed
			can_move = false;

			//positive direction then increase
			player->one_direction++;
		}
		//walk down
		else if (currentKeyStates[SDL_SCANCODE_S] && can_move && !main_map->dwn_obsticle)
		{
			//reset to standard delay
			move_delay = 300;
			//downward facing animation
			main_map->move_row_offscreen(false);
			main_map->animate_player_help(20, 32, 2, 2, 3, 0, 0, 10, move_delay);

			can_move = false;

			//negative direction then decrease
			player->one_direction--;
		}
		//walk left
		else if (currentKeyStates[SDL_SCANCODE_A] && can_move && !main_map->lft_obsticle)
		{
			//reset to standard delay
			move_delay = 300;
			//downward facing animation
			//main_map->move_column_offscreen(true);
			main_map->move_sprite_lr(true);
			main_map->animate_player_help(20, 32, 2, 2, 3, 0, 3, 10, move_delay);

			can_move = false;
		}
		//walk right
		else if (currentKeyStates[SDL_SCANCODE_D] && can_move && !main_map->rght_obsticle)
		{
			//reset to standard delay
			move_delay = 300;
			//downward facing animation
			//main_map->move_column_offscreen(false);
			main_map->move_sprite_lr(false);
			main_map->animate_player_help(20, 32, 2, 2, 3, 0, 9, 15, move_delay);

			can_move = false;
		}
	}
	//save map only for testing purposes
	if (currentKeyStates[SDL_SCANCODE_TAB])
	{
		main_map->SaveMap();
	}
	//load the map that was last saved
	if (currentKeyStates[SDL_SCANCODE_INSERT])
	{
		//since this function takes long, might as well put up a loading sceen
		//SDL_Texture* loader_tx = IMG_LoadTexture(r, "assets/player2.png");
		//SDL_RenderCopy(r, loader_tx, NULL, NULL);
		//SDL_RenderPresent(r);
		//sometimes it runs faster and one does not need to load for long
		main_map->ReadMap();
	}
	//need a condition so that the player can move only every delay milliseconds
	//signed and unsigned mismatch!
	if (SDL_GetTicks() - ticker > move_delay)
	{
		//if enough time has passed player can move and the time changes
		can_move = true;
		ticker = SDL_GetTicks();
	}
	//map rendereing happens here
	main_map->map_render(r);
	//check player collisions here


	if (main_map->collision_player() == Wild)
	{
		//random_encouter();
	}

	//temp battle starter for testing
	if (currentKeyStates[SDL_SCANCODE_B])
	{
		//somthings off about this bool structure
		//but it works for now
		player->battling = true;
		battle_controls(r);

	}
}

//menu function to be implemnted
void Controller::menu_controls()
{
}

//controls for battles
void Controller::battle_controls(SDL_Renderer* r)
{
	//do i want mouse controls or not?
	//enable crossing
	//menu structure for battles
	//should be able to 'switch' on current_rect* to compare to address of other rects for functionality
	//if you add mouse functionality then put it in battle Struct and only pass the mouse ints and return bools
	BattleStruct*  battleObj = new BattleStruct(r);

	//serves a similar function to can_move from main controls, limiting the speed of inputs
	bool prevention = true;

	while (player->battling)
	{
		if (prevention)
		{
			//clear the renderer here or somewhere else?
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

			//move delay for the tile selecting
			move_delay = 600;

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT || currentKeyStates[SDL_SCANCODE_ESCAPE])
				{
					player->battling = false;
				}
			}


			//wasd controls for navagating the menu
			if (currentKeyStates[SDL_SCANCODE_W])
			{
				//select above
				battleObj->currentRect = battleObj->closest_up();
				//reset to standard delay
				prevention = false;
			}
			if (currentKeyStates[SDL_SCANCODE_A])
			{
				//select left by changing what the current rect is
				battleObj->currentRect = battleObj->closest_left();

				prevention = false;
				std::cout << "left x " << battleObj->currentRect->x << "left y " << battleObj->currentRect->y << "left w " << battleObj->currentRect->w << std::endl;
			}
			if (currentKeyStates[SDL_SCANCODE_D])
			{
				//select right
				battleObj->currentRect = battleObj->closest_right();
				//reset to standard delay
				prevention = false;
				std::cout << "right x " << battleObj->currentRect->x << "r y " << battleObj->currentRect->y << "r w " << battleObj->currentRect->w << std::endl;
			}
			if (currentKeyStates[SDL_SCANCODE_S])
			{
				//select below
				battleObj->currentRect = battleObj->closest_down();
				//reset to standard delay
				prevention = false;
			}
			if (currentKeyStates[SDL_SCANCODE_SPACE])
			{
				//select will be a group of if statements that run if current rect has the correct address in it

				//testing quit function
				if (battleObj->currentRect == &battleObj->quit)
				{
					player->battling = false;
					prevention = false;
				}
				if (battleObj->currentRect == &battleObj->bot)
				{
					player->print_stats();
					prevention = false;
				}
				if (battleObj->currentRect == &battleObj->player)
				{
					player->hp_increase(player->mons[0]);
					prevention = false;
				}
				if (battleObj->currentRect == &battleObj->opt)
				{
					player->make_catchable(player->mons[0]);
					prevention = false;
				}
			}
		}

		if (SDL_GetTicks() - ticker > move_delay)
		{
			//if enough time has passed player can move and the tick updates
			prevention = true;
			ticker = SDL_GetTicks();
		}
		
		battleObj->ren_copies(r);
		SDL_RenderPresent(r);
		SDL_RenderClear(r);
	}
	//texture wipe
	battleObj->texture_wipe();
	//disable cursor
}