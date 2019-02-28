#pragma once
#include <SDL.h>
#include <vector>
#include "Enum_Pokes.h"

enum TileType //All the possible tiles should be listed here
{
	Empty,
	Wild,
	Block,
	Door,
	Event
};

class Mapping
{
	//map drawing textures and rectangles
	SDL_Rect draw_rect;
	SDL_Texture* emp_tx;
	SDL_Texture* blk_tx;
	SDL_Texture* wld_tx;
	SDL_Texture* dor_tx;
	SDL_Texture* pnk_tx;

	//player sprite rectangle and textures
	SDL_Texture* sprite_tx;
	SDL_Rect sprite_rect;
	SDL_Rect src_sprite_rect;

	//player sprite rectangle and textures
	SDL_Texture* ntr_tx;
	

	//Stores all the tiles  in the scene
	std::vector<std::vector<TileType>> mTiles;

	//secondarry map storage
	std::vector<std::vector<TileType>> storage;
	
	//subclass for keeping track of items on map
	struct Item_Coords{
		Item_Coords(int _x, int _y, int _id, SDL_Texture* _tx) {
			ix = _x;
			iy = _y;
			iid = _id;
			itx = _tx;
		}
		Item_Coords(){//clear the memory
			SDL_DestroyTexture(itx);
		}
		int ix;
		int iy;
		int iid;
		SDL_Texture* itx;
		SDL_Rect irect;
		SDL_Rect src_ntr_rect;
	};
	//keep a vec with coords and item id and textures in separate vec
	//! try to consolidate the separate vecs into one
	std::vector<Item_Coords> non_trainer_coords;


	//bools for which direction the player has been moving
	bool moving_up, moving_down, moving_left, moving_right;
	bool was_moving_up, was_moving_down, was_moving_left, was_moving_right;

	//these ints are more detailed than the bools
	int left_right_movement;
	int previous_lr_movement;

	//randomize tiles
	void random_tiles_row();

	//functions to tell what is immeadiately around and under the player.
	//just having them return bools might be the better option?
	TileType collision_up();
	TileType collision_down();
	TileType collision_right();
	TileType collision_left();
	
public:
	Mapping();
	Mapping(SDL_Renderer* r);
	~Mapping();

	//functions to tell what is immeadiately around and under the player.
	//just having them return bools might be the better option?
	
	TileType collision_player();

	//bools to block movement
	bool up_obsticle, dwn_obsticle, lft_obsticle, rght_obsticle;

	void map_render(SDL_Renderer* r);
	void move_row_offscreen(bool upward);
	void move_column_offscreen(bool leftward);
	void move_sprite_lr(bool leftward);
	void move_sprite_ud(bool upward);
	void SaveMap();
	void ReadMap();
	void Form_Initial_Map( SDL_Renderer* r);

	void place_door();

	void place_trainers();

	void place_items(int row, int col, SDL_Renderer* r);

	void render_non_players(SDL_Renderer* r);

	void animate_player_help(int w, int h, int offsetX, int offsetY, int frames, int row, int column, int sprites_in_row, int delay);
	void render_ex(SDL_Renderer* r, SDL_Texture* tx, SDL_Rect* src, SDL_Rect* dest, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0, SDL_Point* center = NULL);

	//flipper might not be necessary, which would also make render_ex unnecessary
	SDL_RendererFlip flipper;
};

