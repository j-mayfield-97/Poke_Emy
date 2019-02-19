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
	player = new Trainer("trainer_player", 3, 5, 20);


	Trainer* opp = new Trainer("trainer_opp", 4, 8, 30);
	player->set_opponent(opp);

	//reads to overwrite the test_setup stats
	player->read_stats();
	player->get_opponent()->read_stats();
}

//currently useless but the format of this function is useful
void Controller::test_setup()
{
	//init moves and learn some
	Moves first = Moves("first", 0, 1, 20, 1);
	Moves sec = Moves("sec", 0, 1, 20, 5);
	Moves thr = Moves("thr", 0, 1, 20, 10);
	Moves frth = Moves("frth", 0, 1, 20, 20);

	player->learnMoves(first);
	player->learnMoves(sec);
	player->learnMoves(thr);
	player->learnMoves(frth);

	Mon* mon1 = new Mon();
	Mon* mon2 = new Mon();
	Mon* mon3 = new Mon();

	mon1->learnMoves(first);
	mon1->learnMoves(sec);

	mon2->learnMoves(first);
	mon2->learnMoves(thr);

	mon3->learnMoves(first);
	mon3->learnMoves(frth);

	player->add_mon(mon1);
	player->add_mon(mon2);
	player->add_mon(mon3);

	Trainer* opp = new Trainer("trainer_opp",  4 , 8, 30);

	opp->learnMoves(sec);

	Mon* oppmon1 = new Mon();
	Mon* oppmon2 = new Mon();
	Mon* oppmon3 = new Mon();

	oppmon1->learnMoves(first);
	oppmon1->learnMoves(sec);

	oppmon2->learnMoves(first);
	oppmon2->learnMoves(thr);

	oppmon3->learnMoves(first);
	oppmon3->learnMoves(frth);

	opp->add_mon(oppmon1);
	opp->add_mon(oppmon2);
	opp->add_mon(oppmon3);

	player->set_opponent(opp);
	player->opponent_t = opp;
}



Controller::~Controller()
{
	if(player->get_opponent() != nullptr)
		delete player->get_opponent();

	delete player;
	delete main_map;
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

	//temp test set up to be dleted later
	//test_setup(); 

	

	while (player->battling)
	{
		if (prevention)
		{
			//clear the renderer here or somewhere else?
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

			//move delay for the tile selecting
			move_delay = 500;

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT || currentKeyStates[SDL_SCANCODE_ESCAPE])
				{
					player->battling = false;
				}
			}

			if (currentKeyStates[SDL_SCANCODE_Z])
			{
				//select above
				battleObj->undo_q();
				//reset to standard delay
				prevention = false;
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
			}
			if (currentKeyStates[SDL_SCANCODE_D])
			{
				//select right
				battleObj->currentRect = battleObj->closest_right();
				//reset to standard delay
				prevention = false;
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
				//currently this block is just testing anyfunction that needs testing
				//testing quit function
				if (battleObj->currentRect == &battleObj->quit)
				{
					//triggers quiting battle screen
					//player->battling = false;
					//should pass player stat instead of constant
					battleObj->play_q();
					prevention = false;
				}
				if (battleObj->currentRect == &battleObj->bot)
				{
					//print stats write to the trainer and the mons jsons
					//player->print_stats();
					//player->opponent_t->print_stats();
					/////////////////////
					/// if the actions were not members of classes then i could access their address directly
					///that kinda defeats the spirit of oop though
					battleObj->push_to_q(&Combatant::basic_attack, player, player->get_opponent(), 10);
					prevention = false;
					std::cout << "bot\n";
				}
				if (battleObj->currentRect == &battleObj->player)
				{
					battleObj->push_to_q(&Combatant::basic_defense, player, player->mon_returner(), 30);
					player->mon_cycler();
					prevention = false;
					std::cout << "player\n";
				}
				if (battleObj->currentRect == &battleObj->opt)
				{
					//player->make_catchable(player->mons[0]);
					//player->set_opponent(player->opponent_t->mon_returner());
					//player->attack_creature(player->move_list[0]);
					battleObj->push_to_front(&Combatant::lick_wounds, player, player, 40);
					prevention = false;
					std::cout << "opt\n";
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
	delete battleObj;
	//disable cursor
}