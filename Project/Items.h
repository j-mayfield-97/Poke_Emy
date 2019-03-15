#pragma once
#include <SDL.h>
//previously a sublass in Mapping 

enum ItemType //All the possible items types should be listed here
{
	iEmpty,
	iCoin,
	iMon,
	iTrainer,
	iHeld,
	iLife,
};

struct Items
{
	Items(int _x, int _y, ItemType _id, SDL_Texture* _tx);
	~Items();

	void Set_Animation_Constants(int _w, int _h, int _offsetX, int _offsetY, int _frames, int _row, int _column, int _sprites_in_row, int _delay);

	void animate_help(SDL_Renderer* iren);
	void simple_render(SDL_Renderer* iren);

	void render_ex(SDL_Renderer * r, SDL_Texture * tx, SDL_Rect * src, SDL_Rect * dest, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0.0, SDL_Point* center = NULL);

	//tile place on map
	int ix;
	int iy;

	//ID
	ItemType iid;

	//mind the difference betwwen mapping and battling vals

	//map rendering values
	SDL_Texture* itx;
	SDL_Rect irect;
	SDL_Rect src_irect;
	//battle rendering values
	SDL_Texture* b_itx;
	SDL_Rect b_irect;
	SDL_Rect b_src_irect;

	const char* tool_tip_txt;
	
	int value;

	bool constants_set;

	int w;
	int h;
	int offsetX;
	int offsetY;
	int frames;
	int row;
	int column;
	int sprites_in_row;
	int delay;
};

