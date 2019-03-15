#pragma once
#include "Items.h"


Items::Items(int _x, int _y, ItemType _id, SDL_Texture* _tx) {
	ix = _x;
	iy = _y;
	iid = _id;
	itx = _tx;
	constants_set = false;

	src_irect.x = irect.x = 0;
	src_irect.y = irect.y = 0;
	src_irect.h = irect.h = 0;
	src_irect.w = irect.w = 0;
}

Items::~Items()
{
	//clear the memory
	SDL_DestroyTexture(itx);
	//dont destroy renderer here, will be destroyed on exiting controller
}


//set values to be used in the animation helper
void Items::Set_Animation_Constants(int _w, int _h, int _offsetX, int _offsetY, int _frames, int _row, int _column, int _sprites_in_row, int _delay)
{
	 w = _w;
	 h = _h;
	 offsetX = _offsetX;
	 offsetY = _offsetY;
	 frames = _frames;
	 row = _row;
	 column = _column;
	 sprites_in_row = _sprites_in_row;
	 delay = _delay;

	constants_set = true;
}

//pass renderer here instead of to the contructor
//more robust version of animate_helper taking advantage of Items struct
void Items::animate_help(SDL_Renderer* iren)
{
	//w and h are size of sprites on the sheet in pixels
	//offsets from topleft in pixels
	//frames is how many frames will be in animation
	//row column are where the animation starts i.e. 0th row, 5th column 
	//sprites in row is how many sprites are in the row 
	//and delay is how long the animations will play

	src_irect.y = (row * h) + offsetY;
	src_irect.w = w;
	src_irect.h = h;

	//SDL_GetTicks() method gives us the time in milliseconds
	//'frame' will give us the index of frame we want to render
	//For example, if 'frame' is 2, it will give us the third frame
	int frame = (SDL_GetTicks() / delay) % frames;

	//must wrap around when it get to the end of row
	if (frame + column > sprites_in_row)
	{
		src_irect.x = offsetX;
		src_irect.y += h;
	}
	else
		src_irect.x = (frame + column) * src_irect.w + offsetX;

	render_ex(iren, itx, &src_irect, &irect);
}

//for non animated items
void Items::simple_render(SDL_Renderer* iren)
{
	SDL_RenderCopy(iren, itx, NULL, &irect);
}

void Items::render_ex(SDL_Renderer* r, SDL_Texture* tx, SDL_Rect* src, SDL_Rect* dest, SDL_RendererFlip flip, double angle, SDL_Point* center)
{//Function just to add easy defaults to rendercopyEx 

 //Render to screen 
	SDL_RenderCopyEx(r, tx, src, dest, angle, center, flip);
}