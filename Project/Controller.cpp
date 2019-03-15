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
	main_map->Form_Initial_Map(r);
	//initial draw of the row_size
	main_map->animate_player_help(20, 32, 2, 2, 3, 0, 0, 10, move_delay);

	//instance row_size
	//row_size = new Trainer("trainer_row_size", 3, 5, 20);

	//Trainer* opp = new Trainer("trainer_opp", 4, 8, 30);
	player = new Trainer("Trainer/trainer_player");
	Trainer* opp = new Trainer("Trainer/trainer_opp");

	player->set_current_target(opp);
	player->opponent_t = opp;
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

	player->set_current_target(opp);
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
			main_map->move_sprite_ud(true);
			main_map->animate_player_help(20, 32, 3, -1, 3, 1, 6, 10, move_delay);

			can_move = false;
		}
		if (currentKeyStates[SDL_SCANCODE_S] && can_move && !main_map->dwn_obsticle)
		{
			//downward run animation
			main_map->move_sprite_ud(false);
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
			main_map->move_sprite_ud(true);
			//up facing animation
			main_map->animate_player_help(20, 32, 2, 2, 3, 0, 6, 10, move_delay);

			//prevent movemnt until the delay time has passed
			can_move = false;
		}
		//walk down
		else if (currentKeyStates[SDL_SCANCODE_S] && can_move && !main_map->dwn_obsticle)
		{
			//reset to standard delay
			move_delay = 300;
			//downward facing animation
			main_map->move_sprite_ud(false);
			main_map->animate_player_help(20, 32, 2, 2, 3, 0, 0, 10, move_delay);

			can_move = false;
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
	if (currentKeyStates[SDL_SCANCODE_TAB] && can_move)
	{
		main_map->SaveMap();
	}
	//load the map that was last saved
	if (currentKeyStates[SDL_SCANCODE_INSERT] && can_move)
	{
		main_map->ReadMap();
	}

	//test code to continue through floors
	if (main_map->collision_player() == Door && currentKeyStates[SDL_SCANCODE_Z] && can_move)
	{
		main_map->Form_Initial_Map(r);
		std::cout << "floors : " << player->increment_floor(1) << std::endl;
	}
	//if the row_size hits x over an item pick it up
	//push to item vec and despawn from map
	if (currentKeyStates[SDL_SCANCODE_X] && can_move)
	{
		Items* item = main_map->item_collision();
		if(item != nullptr)
			player->add_item(item);
		delete item;
	}

	//need a condition so that the row_size can move only every delay milliseconds
	//signed and unsigned mismatch!
	if (SDL_GetTicks() - ticker > move_delay)
	{
		//if enough time has passed row_size can move and the time changes
		can_move = true;
		ticker = SDL_GetTicks();
	}
	//map rendereing happens here
	main_map->map_render(r);
	//check row_size collisions here


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
void Controller::menu_controls(SDL_Renderer* r)
{
	menuing = false;
}

//type def for returning battle action functions
void(*player_ability)(Combatant*, Combatant*, int);
//gui and controls for battles includes mouse and keyboard controls
void Controller::battle_controls(SDL_Renderer* r)
{
	BattleStruct* battleObj = new BattleStruct(r);

	//fill ui elements
	battleObj->fill_mons_rect(player, r);
	battleObj->fill_opponent_mons_rect(player->opponent_t, r);
	battleObj->hp_affected(player, r);

	//serves a similar function to can_move from main controls, limiting the speed of inputs
	bool prevention = true;

	//temp test set up to be dleted later
	//test_setup(); 

	//mouse cursor enable
	SDL_ShowCursor(SDL_ENABLE);

	SDL_Point mouse_point = SDL_Point();
	int mouse_x = 0;
	int mouse_y = 0;	

	while (player->battling)
	{
		//find the mouse on the screen
		SDL_GetMouseState(&mouse_x, &mouse_y);
		mouse_point.x = mouse_x;
		mouse_point.y = mouse_y;

		if (prevention)
		{
			//clear the renderer here or somewhere else?
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

			//move delay for the tile selecting
			move_delay = 500;

			while (SDL_PollEvent(&e) != 0)
			{
				//end battle on window close or esc key
				if (e.type == SDL_QUIT || currentKeyStates[SDL_SCANCODE_ESCAPE])
				{
					player->battling = false;
				}
				//mouse controls
				if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONUP)
				{
					//current ect becomes the selectable rect that the cursor is in
					battleObj->currentRect = battleObj->mouse_helper(mouse_point);

					if (e.type == SDL_MOUSEBUTTONUP)
					{
						//end turn play all queued actions
						if (battleObj->currentRect == &battleObj->end_turn_rect)
						{
							battleObj->play_q();
							battleObj->hp_affected(player, r);
							prevention = false;
						}
						//enter the options menu
						if(battleObj->currentRect == &battleObj->options_rect)
						{
							menuing = true;
							menu_controls(r);
							prevention = false;
						}
						//pop the last action added to queue
						if (battleObj->currentRect == &battleObj->undo_rect)
						{
							battleObj->undo_q();
							prevention = false;
						}
						//quit closes the game
						if (battleObj->currentRect == &battleObj->quit_save_rect)
						{
							player->battling = false;
							running = false;
							prevention = false;
						}
						//first opp as target
						if (battleObj->currentRect == &battleObj->first_opp_m.sprite_rect)
						{
							player->selected_target = player->opponent_t->mons[0];
							battleObj->currentTargetRect = &battleObj->first_opp_m.sprite_rect;
							prevention = false;
						}
						//second opp as target
						if (battleObj->currentRect == &battleObj->second_opp_m.sprite_rect)
						{
							player->selected_target = player->opponent_t->mons[1];
							battleObj->currentTargetRect = &battleObj->second_opp_m.sprite_rect;
							prevention = false;
						}
						//thrid opp as target
						if (battleObj->currentRect == &battleObj->third_opp_m.sprite_rect)
						{
							player->selected_target = player->opponent_t->mons[2];
							battleObj->currentTargetRect = &battleObj->third_opp_m.sprite_rect;
							prevention = false;
						}
						//opp as target
						if (battleObj->currentRect == &battleObj->opponent_sprite_rect)
						{
							player->selected_target = player->opponent_t;
							battleObj->currentTargetRect = &battleObj->opponent_sprite_rect;
							prevention = false;
						}
						//first mon top ability
						if (battleObj->currentRect == &battleObj->first_m.top_move_rect)
						{
							battleObj->push_to_q(&Combatant::basic_attack, player->mons[0], player->selected_target, 10);
							battleObj->fill_queue_rect("assets/Xtxt.png", r);
							prevention = false;
						}
						//first mon bottom ability
						if (battleObj->currentRect == &battleObj->first_m.bottom_move_rect)
						{
							battleObj->push_to_q(&Combatant::lick_wounds, player->mons[0], player->mons[0], 10);
							battleObj->fill_queue_rect("assets/Ytxt.png", r);
							prevention = false;
						}
						//second mon top ability
						if (battleObj->currentRect == &battleObj->second_m.top_move_rect)
						{
							battleObj->push_to_q(&Combatant::basic_attack, player->mons[1], player->selected_target, 10);
							battleObj->fill_queue_rect("assets/Xtxt.png", r);
							prevention = false;
						}
						//second mon bottom ability
						if (battleObj->currentRect == &battleObj->second_m.bottom_move_rect)
						{
							battleObj->push_to_q(&Combatant::lick_wounds, player->mons[1], player->mons[1], 10);
							battleObj->fill_queue_rect("assets/Ytxt.png", r);
							prevention = false;
						}
						//third mon top ability
						if (battleObj->currentRect == &battleObj->third_m.top_move_rect)
						{
							battleObj->push_to_q(&Combatant::basic_attack, player->mons[2], player->selected_target, 10);
							battleObj->fill_queue_rect("assets/Xtxt.png", r);
							prevention = false;
						}
						//third mon bottom ability
						if (battleObj->currentRect == &battleObj->third_m.bottom_move_rect)
						{
							battleObj->push_to_q(&Combatant::lick_wounds, player->mons[2], player->mons[2], 10);
							battleObj->fill_queue_rect("assets/Ytxt.png", r);
							prevention = false;
						}
						//player ability 1
						if (battleObj->currentRect == &battleObj->third_m.bottom_move_rect)
						{
							player_ability = &Combatant::lick_wounds;
							//figure out a way to return the ability function
							battleObj->push_to_q(player_ability, player, player->selected_target, 10);
							battleObj->fill_queue_rect("assets/Ytxt.png", r);
							prevention = false;
						}
					}
				}
			}
			//undo last action on z key
			if (currentKeyStates[SDL_SCANCODE_Z])
			{
				battleObj->undo_q();
				prevention = false;
			}
			//wasd controls for navagating the menu
			if (currentKeyStates[SDL_SCANCODE_W])
			{
				//select above
				battleObj->currentRect = battleObj->closest_up();
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
				prevention = false;
			}
			if (currentKeyStates[SDL_SCANCODE_S])
			{
				//select below
				battleObj->currentRect = battleObj->closest_down();
				prevention = false;
			}
			//space key to collect current rect
			if (currentKeyStates[SDL_SCANCODE_SPACE])
			{
				//select will be a group of if statements that run if current rect has the correct address in it
				//currently this block is just testing anyfunction that needs testing
				//end turn 
				if (battleObj->currentRect == &battleObj->end_turn_rect)
				{
					battleObj->play_q();
					prevention = false;
				}
				//items 
				if (battleObj->currentRect == &battleObj->items_rect)
				{
					prevention = false;
				}
				//options menu
				if (battleObj->currentRect == &battleObj->options_rect)
				{
					menuing = true;
					menu_controls(r);
					prevention = false;
				}
			}
		}

		if (SDL_GetTicks() - ticker > move_delay)
		{
			//if enough time has passed row_size can move and the tick updates
			prevention = true;
			ticker = SDL_GetTicks();
		}
		//if opponent faints play win animation and fade to overworld
		//if (!player->get_opponent()->alive_check())
		{
		//	player->battling = false;
		}
		
		battleObj->ren_copies(r);
		battleObj->mon_render(player, player->opponent_t, r);
		SDL_RenderPresent(r);
		SDL_RenderClear(r);
	}
	//texture wipe
	battleObj->texture_wipe();
	delete battleObj;
	//disable cursor
	SDL_ShowCursor(SDL_DISABLE);
}