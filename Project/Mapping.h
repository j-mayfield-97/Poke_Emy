#pragma once
#include <SDL.h>
#include <vector>
#include "Enum_Pokes.h"

enum TileType //All the possible tiles should be listed here
{
	Empty,
	Wild,
	Block,
	OneWay,
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

	//player sprite rectangle and textures
	SDL_Texture* sprite_tx;
	SDL_Rect sprite_rect;
	SDL_Rect src_sprite_rect;

	//Stores all the tiles  in the scene
	std::vector<std::vector<TileType>> mTiles;

	//secondarry map storage
	std::vector<std::vector<TileType>> storage;

	//bools for which direction the player has been moving
	bool moving_up, moving_down, moving_left, moving_right;
	bool was_moving_up, was_moving_down, was_moving_left, was_moving_right;
	int left_right_movement;

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
	void replace_column();
	void SaveMap();
	void ReadMap();
	void Form_Initial_Map();

	void animate_help(int w, int h, int offsetX, int offsetY, int frames, int row, int column, int sprites_in_row, int delay);
	void render_ex(SDL_Renderer* r, SDL_Texture* tx, SDL_Rect* src, SDL_Rect* dest, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0, SDL_Point* center = NULL);

	//flipper might not be necessary, which would also make render_ex unnecessary
	SDL_RendererFlip flipper;
};

