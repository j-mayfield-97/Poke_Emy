#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "../Enum_Pokes.h"
#include <vector>
#include <functional>
#include "../Combatant.h"
//FOR SOME REASON This has access to main's renderer - ren
//get all logic of menu into this class

class BattleStruct
{
	SDL_Texture* under_tx;
	SDL_Texture* hover_tx;
	SDL_Texture* enemy_hover_tx;

	SDL_Texture* pmoves_tx;
	SDL_Texture* player_sprite_tx;
	SDL_Texture* opponent_sprite_tx;
	SDL_Texture* opponent_mons_tx;
	SDL_Texture* options_tx;
	SDL_Texture* items_tx;
	SDL_Texture* end_turn_tx;
	SDL_Texture* quit_save_tx;
	SDL_Texture* mons_tx;
	SDL_Texture* undo_tx;
	SDL_Texture* queue_tx;

	TTF_Font* font;
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;
	SDL_Color color_red;
	SDL_Color color_blk;

	SDL_Texture* pmoves_tff_tx;
	SDL_Texture* opponent_mons_tff_tx;
	SDL_Texture* options_tff_tx;
	SDL_Texture* items_tff_tx;
	SDL_Texture* end_turn_tff_tx;
	SDL_Texture* quit_save_tff_tx;
	SDL_Texture* mons_tff_tx;
	SDL_Texture* undo_tff_tx;
	SDL_Texture* queue_tff_tx;

	//vector for iterating through menu tiles
	std::vector<SDL_Rect*> rect_pntr_vec;

	//holds the battle action order and values attached to them
	std::vector<void(*)(Combatant*, Combatant*, int)> action_queue;
	//combatant queues can be compacted into one later
	std::vector<Combatant*> actor_q;
	std::vector<Combatant*> combatant_q;
	std::vector<int> int_q;
	//texture vector
	std::vector<SDL_Texture*> action_images_q;
	//vector for iterating through menu tiles
	std::vector<SDL_Rect*> rect_pntr_q;

	struct Mon_Block {
		SDL_Texture* sprite_tx;
		SDL_Texture* top_tx;
		SDL_Texture* bottom_tx;
		SDL_Texture* health_tff_tx;
		SDL_Texture* p_h_tff_tx;
		SDL_Rect sprite_rect;
		SDL_Rect top_move_rect;
		SDL_Rect bottom_move_rect;
		SDL_Rect health_rect;
		SDL_Rect p_h_rect;
		~Mon_Block() {
			//delete sprite_tx;
			//delete top_tx;
			//delete bottom_tx;
			//delete health_tff_tx;
		}
	};
	void rect_initer(SDL_Rect* rect, int x, int y, int w, int h,
		SDL_Texture** tx, const char* tx_path,
		SDL_Texture** tff_tx, TTF_Font* fnt, const char* text, SDL_Color col,
		SDL_Renderer* ren);
public:
	BattleStruct();
	BattleStruct(SDL_Renderer *r);
	
	void texture_wipe();
	void ren_copies(SDL_Renderer* r);
	void hovering(SDL_Renderer* r);
	
	SDL_Rect* closest_left();
	SDL_Rect* closest_right();
	SDL_Rect* closest_up();
	SDL_Rect* closest_down();

	SDL_Rect pmoves_rect;
	SDL_Rect player_sprite_rect;
	SDL_Rect opponent_sprite_rect;
	SDL_Rect opponent_mons_rect;
	SDL_Rect options_rect; 
	SDL_Rect items_rect;
	SDL_Rect end_turn_rect;
	SDL_Rect quit_save_rect;
	SDL_Rect mons_rect;
	SDL_Rect undo_rect;
	SDL_Rect queue_rect;
	SDL_Rect* currentRect;
	SDL_Rect* currentTargetRect;

	SDL_Rect* mouse_helper(SDL_Point p);

	void fill_mons_rect(Trainer* tr_ptr, SDL_Renderer* r);
	void fill_opponent_mons_rect(Trainer* tr_ptr, SDL_Renderer* r);
	void hp_affected(Trainer* tr_ptr, SDL_Renderer* r);
	void mon_render(Trainer* tr_ptr, Trainer * opp_tr, SDL_Renderer* r);
	void fill_queue_rect(const char* icon_path, SDL_Renderer* r);
	void fill_items_rect(Trainer* tr_ptr);
	void tool_tipper(Combatant* com);

	//visual studios defaulted all the recntly added functions to be inline 
	//I will trust it and leave them inline for now

	//example
	// a punches b
	//push function*  with the target Combatant* and the stat from the attacker used to determine damage
	void push_to_q(void(*action)(Combatant*, Combatant*, int), Combatant* actor, Combatant* com, int num);
	void push_to_front(void(*action)(Combatant*, Combatant*, int), Combatant* actor, Combatant* com, int num);
	void push_to_back(void(*action)(Combatant*, Combatant*, int), Combatant* actor, Combatant* com, int num);

	void undo_q();
	void play_q();

	Mon_Block first_m;
	Mon_Block second_m;
	Mon_Block third_m;
	Mon_Block first_opp_m;
	Mon_Block second_opp_m;
	Mon_Block third_opp_m;
	//player moves also needs 4 rects and 4 textures
	//Mon_block has a similar form
	Mon_Block pmoves_obj;
};

//should not be used
BattleStruct::BattleStruct() 
{ 
}

//text render based on code from stackexchange user kdyz

BattleStruct::BattleStruct(SDL_Renderer* r)
{
	
	//init rects textures and vectors
	 //load font and set size / color
	 font = TTF_OpenFont("assets/whirab.ttf", 24);
	 color_blk = {0, 0, 0};
	 color_red = { 255, 0, 0 };

	 //queue displays queued actions that will play on the end of turn
	 rect_initer(&queue_rect, 0, 0, screenWidth / 2, screenHeight / 12,
		 NULL, "",
		 &queue_tff_tx, font, "queue", color_blk, r);
	 //player moves to be divided into 4
	 //not selectable so pop right after function
	 rect_initer(&pmoves_rect, 0, queue_rect.h + 5, screenWidth / 4, screenHeight / 5,
				NULL, "",
				&pmoves_tff_tx, font, "p.moves", color_red, r);
	 rect_pntr_vec.pop_back();
	 //sprite for player, !!add a paramter to make it non_selectable , or think of something selecting it does
	 rect_initer(&player_sprite_rect, 0, pmoves_rect.y + pmoves_rect.h + 20, screenWidth / 4, screenHeight - (pmoves_rect.y + pmoves_rect.h + 100),
		 &player_sprite_tx, "assets/Trainer_Back.png",
		 NULL, font, "", color_red, r);
	 //opponent sprite
	 rect_initer(&opponent_sprite_rect, screenWidth - player_sprite_rect.w, 0, player_sprite_rect.w, player_sprite_rect.y,
		 &opponent_sprite_tx, "assets/player2.png",
		 NULL, font, "", color_red, r);
	 //quit , saves the middle of the battle delete upon return
	 rect_initer(&quit_save_rect, player_sprite_rect.w / 2, screenHeight - ( screenHeight / 8), screenWidth / 7, screenHeight / 8,
				NULL, "", 
				&quit_save_tff_tx, font, "save&quit", color_blk, r);
	 //options will lead into another menu, that will inherit features from this
	 rect_initer(&options_rect, quit_save_rect.x + quit_save_rect.w + 5, quit_save_rect.y, quit_save_rect.w, quit_save_rect.h,
		 NULL, "",
		 &options_tff_tx, font, "options", color_blk, r);
	 //undo queue
	 rect_initer(&undo_rect, options_rect.x + options_rect.w + 5, quit_save_rect.y, quit_save_rect.w, quit_save_rect.h,
		 NULL, "",
		 &undo_tff_tx, font, "undo", color_blk, r);
	 //ends turn
	 rect_initer(&end_turn_rect, undo_rect.x + undo_rect.w + 5, quit_save_rect.y, quit_save_rect.w, quit_save_rect.h,
		 NULL, "",
		 &end_turn_tff_tx, font, "end turn", color_blk, r);
	 //items list to be filled by items vector
	 rect_initer(&items_rect, screenWidth - (opponent_sprite_rect.w / 3), screenHeight / 2 , opponent_sprite_rect.w / 3, screenHeight / 2,
			NULL, "",
			&items_tff_tx, font, "items", color_red, r);
	 rect_pntr_vec.pop_back();
	 //place where mons are held // will be divided into 3 slots // not selctable
	 rect_initer(&mons_rect, player_sprite_rect.w + 5, screenHeight - ( options_rect.h + (int)(screenHeight / 2.4 )), screenWidth - (player_sprite_rect.w + items_rect.w + 20), screenHeight / 3,
		 NULL, "",
		 &mons_tff_tx, font, "mons", color_red, r);
	 rect_pntr_vec.pop_back();
	 //where opponent mons are held
	 rect_initer(&opponent_mons_rect, opponent_sprite_rect.x - mons_rect.w * 3/4, queue_rect.h + 20, mons_rect.w / 1.5, (int) (mons_rect.h / 1.5),
				NULL, "",
				&opponent_mons_tff_tx, font, "opp mons", color_red, r);
	 rect_pntr_vec.pop_back();

		//Major Moves will be divided into sub rects
	

	//current rect and target rect starts at player sprite but invisible
	currentRect = &player_sprite_rect;
	currentTargetRect = &player_sprite_rect;

	//overlays and underlays
	under_tx = IMG_LoadTexture(r, "assets/redOverlay.png");
	hover_tx = IMG_LoadTexture(r, "assets/blueOverlay.png");
	enemy_hover_tx = IMG_LoadTexture(r, "assets/blueDots.png");
}

//funtion for helping automate ui rectangles // images and text
void BattleStruct::rect_initer(SDL_Rect* rect, int x, int y, int w, int h, 
										SDL_Texture** tx, const char* tx_path,
										SDL_Texture** tff_tx, TTF_Font* fnt, const char* text, SDL_Color col,
										SDL_Renderer* ren)
{
	//rect = new SDL_Rect();
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;

	if (tx != NULL)
		*tx = IMG_LoadTexture(ren, tx_path);


	//add rect to the vector that allows for keyboard navigation
	rect_pntr_vec.push_back(rect);

	if(tff_tx != NULL)
		*tff_tx = SDL_CreateTextureFromSurface(ren, TTF_RenderText_Solid(fnt, text, col));

}

//clean the textures
void BattleStruct::texture_wipe()
{
	SDL_DestroyTexture(enemy_hover_tx);
	SDL_DestroyTexture(end_turn_tff_tx);
	SDL_DestroyTexture(under_tx);
	SDL_DestroyTexture(hover_tx);
	SDL_DestroyTexture(items_tff_tx);
	//SDL_DestroyTexture(items_tx);
	SDL_DestroyTexture(mons_tff_tx);
	//SDL_DestroyTexture(mons_tx);
	SDL_DestroyTexture(opponent_mons_tff_tx);
	//SDL_DestroyTexture(opponent_mons_tx);
	//SDL_DestroyTexture(undo_tx);
	SDL_DestroyTexture(undo_tff_tx);
	//SDL_DestroyTexture(quit_save_tx);
	SDL_DestroyTexture(quit_save_tff_tx);
	//SDL_DestroyTexture(queue_tx);
	SDL_DestroyTexture(queue_tff_tx);
	//SDL_DestroyTexture(pmoves_tx);
	SDL_DestroyTexture(pmoves_tff_tx);
	SDL_DestroyTexture(player_sprite_tx);
	SDL_DestroyTexture(options_tff_tx);
	//SDL_DestroyTexture(options_tx);
	SDL_DestroyTexture(opponent_sprite_tx);
}

void BattleStruct::hovering(SDL_Renderer * r) // maybe just pass a rect adress instead to set current rect
{
	SDL_RenderCopy(r, hover_tx, NULL, currentRect);
	SDL_RenderCopy(r, enemy_hover_tx, NULL, currentTargetRect);
}

void BattleStruct::ren_copies(SDL_Renderer* r)
{

	hovering(r);

	//underlay for selectable  ui items
	SDL_RenderCopy(r, under_tx, NULL, &queue_rect);
	//SDL_RenderCopy(r, under_tx, NULL, &pmoves_rect);
	SDL_RenderCopy(r, under_tx, NULL, &player_sprite_rect);
	SDL_RenderCopy(r, under_tx, NULL, &opponent_sprite_rect);
	//SDL_RenderCopy(r, under_tx, NULL, &opponent_mons_rect);
	//SDL_RenderCopy(r, under_tx, NULL, &mons_rect);
	//SDL_RenderCopy(r, under_tx, NULL, &items_rect);
	SDL_RenderCopy(r, under_tx, NULL, &options_rect);
	SDL_RenderCopy(r, under_tx, NULL, &end_turn_rect);
	SDL_RenderCopy(r, under_tx, NULL, &quit_save_rect);
	SDL_RenderCopy(r, under_tx, NULL, &undo_rect);

	SDL_RenderCopy(r, queue_tff_tx, NULL, &queue_rect);
	SDL_RenderCopy(r, pmoves_tff_tx, NULL, &pmoves_rect);
	SDL_RenderCopy(r, player_sprite_tx, NULL, &player_sprite_rect);
	SDL_RenderCopy(r, opponent_sprite_tx, NULL, &opponent_sprite_rect);
	SDL_RenderCopy(r, opponent_mons_tff_tx, NULL, &opponent_mons_rect);
	SDL_RenderCopy(r, mons_tff_tx, NULL, &mons_rect);
	SDL_RenderCopy(r, items_tff_tx, NULL, &items_rect);
	SDL_RenderCopy(r, options_tff_tx, NULL, &options_rect);
	SDL_RenderCopy(r, end_turn_tff_tx, NULL, &end_turn_rect);
	SDL_RenderCopy(r, quit_save_tff_tx, NULL, &quit_save_rect);
	SDL_RenderCopy(r, undo_tff_tx, NULL, &undo_rect);

	//loop through queue and render
	for (int i = 0; i < action_images_q.size(); i++)
	{
		SDL_RenderCopy(r, action_images_q[i], NULL, rect_pntr_q[i]);
	}
	
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

inline SDL_Rect* BattleStruct::mouse_helper(SDL_Point p)
{
	//store the current rect to return if outcomes are un sensible
	SDL_Rect* store = currentRect;

	for (SDL_Rect* i : rect_pntr_vec)
	{
		//if point in rect then return that rect
		if (SDL_PointInRect(&p, i))
		{
			return i;
		}
	}
	//keep the current rect if pointer not in any
	return store;
}
//init and fills player side mon rects with icon sprites and health rects, also handles player health and sprite and pmoves rect
inline void BattleStruct::fill_mons_rect(Trainer * tr_ptr, SDL_Renderer* r)
{
	//load trainer sprite here it's rect is handled in the init function
	player_sprite_tx = IMG_LoadTexture(r, tr_ptr->inBattlePath);

	//load icon textures for mons rect and lastly pmoves
	first_m.sprite_tx = IMG_LoadTexture(r, tr_ptr->mons[0]->inBattlePath);
	first_m.top_tx = IMG_LoadTexture(r, tr_ptr->mons[0]->ability_path1);
	first_m.bottom_tx = IMG_LoadTexture(r, tr_ptr->mons[0]->ability_path2);

	second_m.sprite_tx = IMG_LoadTexture(r, tr_ptr->mons[1]->inBattlePath);
	second_m.top_tx = IMG_LoadTexture(r, tr_ptr->mons[1]->ability_path1);
	second_m.bottom_tx = IMG_LoadTexture(r, tr_ptr->mons[1]->ability_path2);

	third_m.sprite_tx = IMG_LoadTexture(r, tr_ptr->mons[2]->inBattlePath);
	third_m.top_tx = IMG_LoadTexture(r, tr_ptr->mons[2]->ability_path1);
	third_m.bottom_tx = IMG_LoadTexture(r, tr_ptr->mons[2]->ability_path2);

	//pmoves textures
	pmoves_obj.sprite_tx = IMG_LoadTexture(r, tr_ptr->t_ability_path1);
	pmoves_obj.top_tx = IMG_LoadTexture(r, tr_ptr->t_ability_path2);
	pmoves_obj.bottom_tx = IMG_LoadTexture(r, tr_ptr->t_ability_path3);
	pmoves_obj.health_tff_tx = IMG_LoadTexture(r, tr_ptr->t_ability_path4);

	//first spot
	first_m.sprite_rect.x = mons_rect.x;
	first_m.sprite_rect.y = mons_rect.y + 5;
	first_m.sprite_rect.w = mons_rect.w / 4 ;
	first_m.sprite_rect.h = mons_rect.h - 10;

	first_m.top_move_rect.x = first_m.sprite_rect.x - 3;
	first_m.top_move_rect.y = first_m.sprite_rect.y - 3;
	first_m.top_move_rect.w = first_m.sprite_rect.w / 2;
	first_m.top_move_rect.h = first_m.sprite_rect.h / 6;

	first_m.bottom_move_rect.x = first_m.sprite_rect.x + first_m.sprite_rect.w - 3;
	first_m.bottom_move_rect.y = first_m.sprite_rect.y + first_m.sprite_rect.h - 3 ;
	first_m.bottom_move_rect.w = first_m.top_move_rect.w;
	first_m.bottom_move_rect.h = first_m.top_move_rect.h;

	first_m.health_rect.x = first_m.sprite_rect.x - 3;
	first_m.health_rect.y = first_m.sprite_rect.y + first_m.sprite_rect.h + 3;
	first_m.health_rect.w = first_m.sprite_rect.w;
	first_m.health_rect.h = first_m.top_move_rect.h;

	//second spot
	second_m.sprite_rect.x = mons_rect.x + (mons_rect.w  / 3);
	second_m.sprite_rect.y = mons_rect.y + 5;
	second_m.sprite_rect.w = mons_rect.w / 4;
	second_m.sprite_rect.h = mons_rect.h - 10;

	second_m.top_move_rect.x = second_m.sprite_rect.x - 3;
	second_m.top_move_rect.y = second_m.sprite_rect.y - 3;
	second_m.top_move_rect.w = second_m.sprite_rect.w / 2;
	second_m.top_move_rect.h = second_m.sprite_rect.h / 6;

	second_m.bottom_move_rect.x = second_m.sprite_rect.x + second_m.sprite_rect.w - 3;
	second_m.bottom_move_rect.y = second_m.sprite_rect.y + second_m.sprite_rect.h - 3;
	second_m.bottom_move_rect.w = second_m.top_move_rect.w;
	second_m.bottom_move_rect.h = second_m.top_move_rect.h;

	second_m.health_rect.x = second_m.sprite_rect.x - 3;
	second_m.health_rect.y = second_m.sprite_rect.y + second_m.sprite_rect.h + 3;
	second_m.health_rect.w = second_m.sprite_rect.w;
	second_m.health_rect.h = second_m.top_move_rect.h;
	//third spot
	third_m.sprite_rect.x = mons_rect.x + ((mons_rect.w * 2) / 3);
	third_m.sprite_rect.y = mons_rect.y + 5;
	third_m.sprite_rect.w = mons_rect.w / 4;
	third_m.sprite_rect.h = mons_rect.h - 10;

	third_m.top_move_rect.x = third_m.sprite_rect.x - 3;
	third_m.top_move_rect.y = third_m.sprite_rect.y - 3;
	third_m.top_move_rect.w = third_m.sprite_rect.w / 2;
	third_m.top_move_rect.h = third_m.sprite_rect.h / 6;

	third_m.bottom_move_rect.x = third_m.sprite_rect.x + third_m.sprite_rect.w - 3;
	third_m.bottom_move_rect.y = third_m.sprite_rect.y + third_m.sprite_rect.h - 3;
	third_m.bottom_move_rect.w = third_m.top_move_rect.w;
	third_m.bottom_move_rect.h = third_m.top_move_rect.h;

	third_m.health_rect.x = third_m.sprite_rect.x - 3;
	third_m.health_rect.y = third_m.sprite_rect.y + third_m.sprite_rect.h - 3;
	third_m.health_rect.w = third_m.sprite_rect.w;
	third_m.health_rect.h = third_m.top_move_rect.h;

	//pmoves ability rects placed in clockwise order
	pmoves_obj.sprite_rect.x = pmoves_rect.x;
	pmoves_obj.sprite_rect.y = pmoves_rect.y;
	pmoves_obj.sprite_rect.w = pmoves_rect.w / 2;
	pmoves_obj.sprite_rect.h = pmoves_rect.h / 2;

	pmoves_obj.top_move_rect.x = pmoves_obj.sprite_rect.x + pmoves_obj.sprite_rect.w;
	pmoves_obj.top_move_rect.y = pmoves_obj.sprite_rect.y;
	pmoves_obj.top_move_rect.w = pmoves_obj.sprite_rect.w;
	pmoves_obj.top_move_rect.h = pmoves_obj.sprite_rect.h;

	pmoves_obj.bottom_move_rect.x = pmoves_obj.sprite_rect.x + pmoves_obj.sprite_rect.w;
	pmoves_obj.bottom_move_rect.y = pmoves_obj.sprite_rect.y + pmoves_obj.sprite_rect.h;
	pmoves_obj.bottom_move_rect.w = pmoves_obj.top_move_rect.w;
	pmoves_obj.bottom_move_rect.h = pmoves_obj.top_move_rect.h;

	pmoves_obj.health_rect.x = pmoves_obj.sprite_rect.x;
	pmoves_obj.health_rect.y = pmoves_obj.sprite_rect.y + pmoves_obj.sprite_rect.h;
	pmoves_obj.health_rect.w = pmoves_obj.sprite_rect.w;
	pmoves_obj.health_rect.h = pmoves_obj.top_move_rect.h;
	
	//health of player rect
	pmoves_obj.p_h_rect.w = player_sprite_rect.w;
	pmoves_obj.p_h_rect.h = player_sprite_rect.h / 8;
	pmoves_obj.p_h_rect.x = player_sprite_rect.x;
	pmoves_obj.p_h_rect.y = player_sprite_rect.y + player_sprite_rect.h - (pmoves_obj.p_h_rect.h / 2);

	//make abilities and moves selectable
	rect_pntr_vec.push_back(&first_m.top_move_rect);
	rect_pntr_vec.push_back(&first_m.bottom_move_rect);
	rect_pntr_vec.push_back(&second_m.top_move_rect);
	rect_pntr_vec.push_back(&second_m.bottom_move_rect);
	rect_pntr_vec.push_back(&third_m.top_move_rect);
	rect_pntr_vec.push_back(&third_m.bottom_move_rect);

	rect_pntr_vec.push_back(&pmoves_obj.sprite_rect);
	rect_pntr_vec.push_back(&pmoves_obj.health_rect);
	rect_pntr_vec.push_back(&pmoves_obj.top_move_rect);
	rect_pntr_vec.push_back(&pmoves_obj.bottom_move_rect);

	//fill items rect
	fill_items_rect(tr_ptr);
}
//same as fill mon frect but hor opponent side
inline void BattleStruct::fill_opponent_mons_rect(Trainer * tr_ptr, SDL_Renderer* r)
{
	opponent_sprite_tx = IMG_LoadTexture(r, tr_ptr->inBattlePath);
	
	//load textures for mons rect 
	first_opp_m.sprite_tx = IMG_LoadTexture(r, tr_ptr->mons[0]->inBattlePath);
	first_opp_m.top_tx = IMG_LoadTexture(r, tr_ptr->mons[0]->ability_path1);
	first_opp_m.bottom_tx = IMG_LoadTexture(r, tr_ptr->mons[0]->ability_path2);

	second_opp_m.sprite_tx = IMG_LoadTexture(r, tr_ptr->mons[1]->inBattlePath);
	second_opp_m.top_tx = IMG_LoadTexture(r, tr_ptr->mons[1]->ability_path1);
	second_opp_m.bottom_tx = IMG_LoadTexture(r, tr_ptr->mons[1]->ability_path2);

	third_opp_m.sprite_tx = IMG_LoadTexture(r, tr_ptr->mons[2]->inBattlePath);
	third_opp_m.top_tx = IMG_LoadTexture(r, tr_ptr->mons[2]->ability_path1);
	third_opp_m.bottom_tx = IMG_LoadTexture(r, tr_ptr->mons[2]->ability_path2);

	//first spot
	first_opp_m.sprite_rect.x = opponent_mons_rect.x;
	first_opp_m.sprite_rect.y = opponent_mons_rect.y + 5;
	first_opp_m.sprite_rect.w = opponent_mons_rect.w / 4;
	first_opp_m.sprite_rect.h = opponent_mons_rect.h - 10;

	first_opp_m.top_move_rect.x = first_opp_m.sprite_rect.x - 3;
	first_opp_m.top_move_rect.y = first_opp_m.sprite_rect.y - 3;
	first_opp_m.top_move_rect.w = first_opp_m.sprite_rect.w / 2;
	first_opp_m.top_move_rect.h = first_opp_m.sprite_rect.h / 5;

	first_opp_m.bottom_move_rect.x = first_opp_m.sprite_rect.x + first_opp_m.sprite_rect.w - 3;
	first_opp_m.bottom_move_rect.y = first_opp_m.sprite_rect.y + first_opp_m.sprite_rect.h - 3;
	first_opp_m.bottom_move_rect.w = first_opp_m.top_move_rect.w;
	first_opp_m.bottom_move_rect.h = first_opp_m.top_move_rect.h;

	first_opp_m.health_rect.x = first_opp_m.sprite_rect.x - 3;
	first_opp_m.health_rect.y = first_opp_m.sprite_rect.y - first_opp_m.top_move_rect.h - 3;
	first_opp_m.health_rect.w = first_opp_m.sprite_rect.w;
	first_opp_m.health_rect.h = first_opp_m.top_move_rect.h;

	//second spot
	second_opp_m.sprite_rect.x = opponent_mons_rect.x + (opponent_mons_rect.w / 3);
	second_opp_m.sprite_rect.y = opponent_mons_rect.y + 5;
	second_opp_m.sprite_rect.w = opponent_mons_rect.w / 4;
	second_opp_m.sprite_rect.h = opponent_mons_rect.h - 10;

	second_opp_m.top_move_rect.x = second_opp_m.sprite_rect.x - 3;
	second_opp_m.top_move_rect.y = second_opp_m.sprite_rect.y - 3;
	second_opp_m.top_move_rect.w = second_opp_m.sprite_rect.w / 2;
	second_opp_m.top_move_rect.h = second_opp_m.sprite_rect.h / 5;

	second_opp_m.bottom_move_rect.x = second_opp_m.sprite_rect.x + second_opp_m.sprite_rect.w - 3;
	second_opp_m.bottom_move_rect.y = second_opp_m.sprite_rect.y + second_opp_m.sprite_rect.h - 3;
	second_opp_m.bottom_move_rect.w = second_opp_m.top_move_rect.w;
	second_opp_m.bottom_move_rect.h = second_opp_m.top_move_rect.h;

	second_opp_m.health_rect.x = second_opp_m.sprite_rect.x - 3;
	second_opp_m.health_rect.y = second_opp_m.sprite_rect.y - second_opp_m.top_move_rect.h - 3;
	second_opp_m.health_rect.w = second_opp_m.sprite_rect.w;
	second_opp_m.health_rect.h = second_opp_m.top_move_rect.h;

	//third spot
	third_opp_m.sprite_rect.x = opponent_mons_rect.x + ((opponent_mons_rect.w * 2) / 3);
	third_opp_m.sprite_rect.y = opponent_mons_rect.y + 5;
	third_opp_m.sprite_rect.w = opponent_mons_rect.w / 4;
	third_opp_m.sprite_rect.h = opponent_mons_rect.h - 10;

	third_opp_m.top_move_rect.x = third_opp_m.sprite_rect.x - 3;
	third_opp_m.top_move_rect.y = third_opp_m.sprite_rect.y - 3;
	third_opp_m.top_move_rect.w = third_opp_m.sprite_rect.w / 2;
	third_opp_m.top_move_rect.h = third_opp_m.sprite_rect.h / 5;

	third_opp_m.bottom_move_rect.x = third_opp_m.sprite_rect.x + third_opp_m.sprite_rect.w - 3;
	third_opp_m.bottom_move_rect.y = third_opp_m.sprite_rect.y + third_opp_m.sprite_rect.h - 3;
	third_opp_m.bottom_move_rect.w = third_opp_m.top_move_rect.w;
	third_opp_m.bottom_move_rect.h = third_opp_m.top_move_rect.h;

	third_opp_m.health_rect.x = third_opp_m.sprite_rect.x - 3;
	third_opp_m.health_rect.y = third_opp_m.sprite_rect.y - third_opp_m.top_move_rect.h - 3;
	third_opp_m.health_rect.w = third_opp_m.sprite_rect.w;
	third_opp_m.health_rect.h = third_opp_m.top_move_rect.h;

	//opp health rect
	third_opp_m.p_h_rect.w = opponent_sprite_rect.w;
	third_opp_m.p_h_rect.h = opponent_sprite_rect.h / 6;
	third_opp_m.p_h_rect.x = opponent_sprite_rect.x;
	third_opp_m.p_h_rect.y = opponent_sprite_rect.y;

	//make sprites selectable
	rect_pntr_vec.push_back(&first_opp_m.sprite_rect);
	rect_pntr_vec.push_back(&second_opp_m.sprite_rect);
	rect_pntr_vec.push_back(&third_opp_m.sprite_rect);
}

//creates the text textures for the mons
//this function must be manually run after anny battle function that affects health 
inline void BattleStruct::hp_affected(Trainer * tr_ptr, SDL_Renderer * r)
{
	//make sdl color on the green to black gradient depending on how much health is left out of max health
	SDL_Color col0 = {0, 200 * ((float)tr_ptr->mons[0]->get_hp() / (float)tr_ptr->mons[0]->get_max_hp()), 0};
	SDL_Color col1 = {0, 200 * ((float)tr_ptr->mons[1]->get_hp() / (float)tr_ptr->mons[1]->get_max_hp()), 0};
	SDL_Color col2 = {0, 200 * ((float)tr_ptr->mons[2]->get_hp() / (float)tr_ptr->mons[2]->get_max_hp()), 0};
	SDL_Color col3 = {0, 200 * ((float)tr_ptr->get_hp() / (float)tr_ptr->get_max_hp()), 0};

	SDL_Color col0opp = { 0, 200 * ((float)tr_ptr->opponent_t->mons[0]->get_hp() / (float)tr_ptr->opponent_t->mons[0]->get_max_hp()), 0 };
	SDL_Color col1opp = { 0, 200 * ((float)tr_ptr->opponent_t->mons[1]->get_hp() / (float)tr_ptr->opponent_t->mons[1]->get_max_hp()), 0 };
	SDL_Color col2opp = { 0, 200 * ((float)tr_ptr->opponent_t->mons[2]->get_hp() / (float)tr_ptr->opponent_t->mons[2]->get_max_hp()), 0 };
	SDL_Color col3opp = { 0, 200 * ((float)tr_ptr->opponent_t->get_hp() / (float)tr_ptr->opponent_t->get_max_hp()), 0 };

	///problem never writing to opp_mons
	first_m.health_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->mons[0]->health_string(), col0));
	second_m.health_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->mons[1]->health_string(), col1));
	third_m.health_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->mons[2]->health_string(), col2));
	pmoves_obj.p_h_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->health_string(), col3));

	first_opp_m.health_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->opponent_t->mons[0]->health_string(), col0opp));
	second_opp_m.health_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->opponent_t->mons[1]->health_string(), col1opp));
	third_opp_m.health_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->opponent_t->mons[2]->health_string(), col2opp));
	third_opp_m.p_h_tff_tx = SDL_CreateTextureFromSurface(r, TTF_RenderText_Solid(font, tr_ptr->opponent_t->health_string(), col3opp));
}

inline void BattleStruct::mon_render(Trainer * tr_ptr, Trainer * opp_tr, SDL_Renderer* r)
{
	//go through all the mons every frame render their sprites and display their current health as text	
	SDL_RenderCopy(r, first_m.sprite_tx, NULL, &first_m.sprite_rect);
	SDL_RenderCopy(r, first_m.top_tx, NULL, &first_m.top_move_rect);
	SDL_RenderCopy(r, first_m.bottom_tx, NULL, &first_m.bottom_move_rect);
	SDL_RenderCopy(r, first_m.health_tff_tx, NULL, &first_m.health_rect);

	SDL_RenderCopy(r, second_m.sprite_tx, NULL, &second_m.sprite_rect);
	SDL_RenderCopy(r, second_m.top_tx, NULL, &second_m.top_move_rect);
	SDL_RenderCopy(r, second_m.bottom_tx, NULL, &second_m.bottom_move_rect);
	SDL_RenderCopy(r, second_m.health_tff_tx, NULL, &second_m.health_rect);

	SDL_RenderCopy(r, third_m.sprite_tx, NULL, &third_m.sprite_rect);
	SDL_RenderCopy(r, third_m.top_tx, NULL, &third_m.top_move_rect);
	SDL_RenderCopy(r, third_m.bottom_tx, NULL, &third_m.bottom_move_rect);
	SDL_RenderCopy(r, third_m.health_tff_tx, NULL, &third_m.health_rect);

	SDL_RenderCopy(r, first_opp_m.sprite_tx, NULL, &first_opp_m.sprite_rect);
	SDL_RenderCopy(r, first_opp_m.top_tx, NULL, &first_opp_m.top_move_rect);
	SDL_RenderCopy(r, first_opp_m.bottom_tx, NULL, &first_opp_m.bottom_move_rect);
	SDL_RenderCopy(r, first_opp_m.health_tff_tx, NULL, &first_opp_m.health_rect);

	SDL_RenderCopy(r, second_opp_m.sprite_tx, NULL, &second_opp_m.sprite_rect);
	SDL_RenderCopy(r, second_opp_m.top_tx, NULL, &second_opp_m.top_move_rect);
	SDL_RenderCopy(r, second_opp_m.bottom_tx, NULL, &second_opp_m.bottom_move_rect);
	SDL_RenderCopy(r, second_opp_m.health_tff_tx, NULL, &second_opp_m.health_rect);

	SDL_RenderCopy(r, third_opp_m.sprite_tx, NULL, &third_opp_m.sprite_rect);
	SDL_RenderCopy(r, third_opp_m.top_tx, NULL, &third_opp_m.top_move_rect);
	SDL_RenderCopy(r, third_opp_m.bottom_tx, NULL, &third_opp_m.bottom_move_rect);
	SDL_RenderCopy(r, third_opp_m.health_tff_tx, NULL, &third_opp_m.health_rect);
	SDL_RenderCopy(r, third_opp_m.p_h_tff_tx, NULL, &third_opp_m.p_h_rect);

	SDL_RenderCopy(r, pmoves_obj.p_h_tff_tx, NULL, &pmoves_obj.p_h_rect);
	SDL_RenderCopy(r, pmoves_obj.sprite_tx, NULL, &pmoves_obj.sprite_rect);
	SDL_RenderCopy(r, pmoves_obj.top_tx, NULL, &pmoves_obj.top_move_rect);
	SDL_RenderCopy(r, pmoves_obj.bottom_tx, NULL, &pmoves_obj.bottom_move_rect);
	SDL_RenderCopy(r, pmoves_obj.health_tff_tx, NULL, &pmoves_obj.health_rect);

	//loop through items and render
	for (auto item : tr_ptr->trainer_items)
	{
		item.irect = item.b_irect;
		SDL_RenderCopy(r, third_opp_m.bottom_tx, NULL, &item.irect);
		if (item.constants_set)
			item.animate_help(r);
		else
			item.simple_render(r);
	}

}

//must be manually added after any function the queues an action
inline void BattleStruct::fill_queue_rect(const char * icon_path, SDL_Renderer* r)
{
	SDL_Texture* tx = IMG_LoadTexture(r, icon_path);
	SDL_Rect* rct = new SDL_Rect;

	//ints to init rects
	int qw = queue_rect.w / 10;
	int qh = queue_rect.h;

	int qx = queue_rect.x + (qw * action_images_q.size());
	int qy = queue_rect.y;

	rct->w = qw;
	rct->h = qh;
	rct->x = qx;
	rct->y = qy;

	//push to rect queue and texture queue
	if (action_images_q.size() < 10)
	{
		action_images_q.push_back(tx);
		rect_pntr_q.push_back(rct);
	}	
}

inline void BattleStruct::fill_items_rect(Trainer * tr_ptr)
{
	for (int i = 0; i < tr_ptr->trainer_items.size(); i++)
	{
		tr_ptr->trainer_items[i].b_irect.w = items_rect.w;
		tr_ptr->trainer_items[i].b_irect.h = items_rect.h / 10;
		tr_ptr->trainer_items[i].b_irect.x = items_rect.x;
		tr_ptr->trainer_items[i].b_irect.y = items_rect.y + (i * tr_ptr->trainer_items[i].b_irect.h);

		rect_pntr_vec.push_back(&tr_ptr->trainer_items[i].b_irect);
	}
}

inline void BattleStruct::tool_tipper(Combatant * com)
{
	//switch on current rect and display text
}

//main function to add an action to the queue actions will play out in order 
inline void BattleStruct::push_to_q(void(*action)(Combatant*, Combatant*, int), Combatant* actor, Combatant* com, int num)
{
	//i dont know what actions to prohibit for now so all actions are allowed 
	action_queue.push_back(action);
	actor_q.push_back(actor);
	combatant_q.push_back(com);
	int_q.push_back(num);
}
//gives actions priority for passive items and the like
inline void BattleStruct::push_to_front(void(*action)(Combatant*, Combatant *, int), Combatant* actor, Combatant* com, int num)
{
	//giving priority to moves or actions cannot be don by the player
	action_queue.insert(action_queue.begin(), action);
	actor_q.insert(actor_q.begin(), actor);
	combatant_q.insert(combatant_q.begin(), com);
	int_q.insert(int_q.begin(), num);
}

//no use for now
//actions that occur after the turn is over
//would need a separate queue
inline void BattleStruct::push_to_back(void(*action)(Combatant*, Combatant *, int), Combatant* actor, Combatant* com, int num)
{
}

inline void BattleStruct::undo_q()
{
	if (action_images_q.size() != action_queue.size())
	{
		SDL_assert("function queued without fill_queue_rect, check all battle functions in controls");
		return;
	}
	//small error handling to not pop an empty queue
	if (action_queue.empty())
	{
		SDL_assert("battle queue is empty");
			return;
	}
	//possibly another feature could be to pass the function that should be undone
	action_queue.pop_back();
	actor_q.pop_back();
	combatant_q.pop_back();
	int_q.pop_back();
	action_images_q.pop_back();
	rect_pntr_q.pop_back();
	
}

inline void BattleStruct::play_q()
{
	//have a confirmation here

	//all three queues should remain the same size
	for (int i = 0; i < action_queue.size(); i++)
	{
		action_queue[i]((actor_q[i]), (combatant_q[i]), (int_q[i]));
	}

	action_queue.clear();
	actor_q.clear();
	combatant_q.clear();
	int_q.clear();
	action_images_q.clear();
	rect_pntr_q.clear();
}
