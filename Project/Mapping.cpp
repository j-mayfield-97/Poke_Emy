#pragma once

#include "Mapping.h"
#include <random>
#include <iostream>
#include <fstream> 
#include <SDL_image.h>


Mapping::Mapping()
{
}

Mapping::Mapping(SDL_Renderer* r)
{
	//init drawing rect
	draw_rect.x = 0;
	draw_rect.y = 0;
	draw_rect.w = tile_size;
	draw_rect.h = tile_size;

	//init sprite sheet rect and texture
	sprite_tx = IMG_LoadTexture(r, "assets/player_sprite.png");
	src_sprite_rect.x = 0;
	src_sprite_rect.y = 0;
	src_sprite_rect.w = 0;
	src_sprite_rect.h = 0;

	//init visible sprite rect
	sprite_rect.x = screenWidth / 2;
	sprite_rect.y = screenHeight / 2;
	sprite_rect.w = tile_size;
	sprite_rect.h = tile_size;

	//flip the texture on rectangle
	flipper = SDL_FLIP_NONE;

	//int for detrmining movement left and right
	left_right_movement = 0;

	//init textures
	//I'll need  function to cut through the new sprite sheets
	emp_tx = IMG_LoadTexture(r, "assets/wall32.png");
	blk_tx = IMG_LoadTexture(r, "assets/water32.png");
	wld_tx = IMG_LoadTexture(r, "assets/grass32.png");
}

//clean up 
Mapping::~Mapping()
{
	SDL_DestroyTexture(emp_tx);
	SDL_DestroyTexture(blk_tx);
	SDL_DestroyTexture(wld_tx);
}

//this set of functions is to sheck what tiles are immeadiately around the player
//the player is always in the middle so whats around the player is the rows above and below and columns to the right and left
//change this !!! THE PLAYER is no longer always in the middle
//theres also danger of going out of bounds!!!!!! unlikely but dangerous
TileType Mapping::collision_up()
{
	int sprite_row = sprite_rect.y / tile_size;
	int sprite_column = sprite_rect.x / tile_size;

	return mTiles[sprite_row - 1][sprite_column];
}

TileType Mapping::collision_down()
{
	int sprite_row = sprite_rect.y / tile_size;
	int sprite_column = sprite_rect.x / tile_size;

	return mTiles[sprite_row + 1][sprite_column];
}
TileType Mapping::collision_right()
{
	int sprite_row = sprite_rect.y / tile_size;
	int sprite_column = sprite_rect.x / tile_size;

	return mTiles[sprite_row ][sprite_column + 1];
}
TileType Mapping::collision_left()
{
	int sprite_row = sprite_rect.y / tile_size;
	int sprite_column = sprite_rect.x / tile_size;

	return mTiles[sprite_row][sprite_column - 1];
}

//this function will check all around the player and return whats under the player
//if the tile is not passable then the corresponding bool gets set to true
TileType Mapping::collision_player()
{
	//up
	if (collision_up() == Block)
		up_obsticle = true;
	else
		up_obsticle = false;
	//down
	if (collision_down() == Block)
		dwn_obsticle = true;
	else
		dwn_obsticle = false;
	//right
	if (collision_right() == Block)
		rght_obsticle = true;
	else
		rght_obsticle = false;
	//left
	if (collision_left() == Block)
		lft_obsticle = true;
	else
		lft_obsticle = false;

	//returns whats under the player

	int sprite_row = sprite_rect.y / tile_size;
	int sprite_column = sprite_rect.x / tile_size;

	return mTiles[sprite_row][sprite_column];
}

//You should have this function retruning a row instead of pushing it back in the function, it'd be more useful that way
void Mapping::random_tiles_row()
{
	//index i is how many tiles that will be needed in a single column
	//this should be a constant
	int i = screenWidth / tile_size;
	//random int 
	int random_int;

	//column array to keep tiles
	std::vector<TileType> row;
	
	//loop through the row size and get a random number
	for (; i > 0; i--)
	{
		//rand generator
		random_int = rand() % 100;

		//percent ranges determine what tile gets pushed to the row vector
		//empty should be the most prevalent
		if (random_int <= 50)
			row.push_back(Empty);
		if (random_int > 50 && random_int <= 60)
			row.push_back(Block);
		if (random_int > 60)
			row.push_back(Wild);		
	}
	//make a wall on the left and right sides
	row[0] = Block;
	row[row.size() - 2] = Block;
	//push that new row to the map
	mTiles.push_back(row);
}

//Make a full map to start with
void Mapping::Form_Initial_Map()
{
	//interator i is how many tiles that will be needed in a single column
	//this should be a constant
	int i = screenHeight / tile_size;

	//loop through height of window and add random rows
	for (; i > 0; i--)
	{
		random_tiles_row();
	}
}
//move thorugh sprite sheet to give the illusion of animations
void Mapping::animate_player_help(int w, int h, int offsetX, int offsetY, int frames, int row, int column, int sprites_in_row, int delay)
{
	//w and h are size of sprites on the sheet
	//offsets from topleft and right
	//frames is how many frames will be in animation
	//row column are where the animation starts i.e. 0th row, 5th column
	//sprites in row is how many sprites are in the row
	//and delay is how long the animations will play

	src_sprite_rect.y = (row * h) + offsetY ;
	src_sprite_rect.w = w;
	src_sprite_rect.h = h;

	//SDL_GetTicks() method gives us the time in milliseconds
	//'frame' will give us the index of frame we want to render
	//For example, if 'frame' is 2, it will give us the third frame
	int frame = (SDL_GetTicks() / delay) % frames;

	//must wrap around when it get to the end on sheet
	if (frame + column > sprites_in_row)
	{
		src_sprite_rect.x = offsetX;
		src_sprite_rect.y += h;
	}
	else
		src_sprite_rect.x = (frame + column) * src_sprite_rect.w + offsetX;
}

void Mapping::render_ex(SDL_Renderer* r, SDL_Texture* tx, SDL_Rect* src, SDL_Rect* dest, SDL_RendererFlip flip, double angle, SDL_Point* center)
{//Function just to add easy defaults to rendercopyEx 
	
	//Render to screen 
	SDL_RenderCopyEx(r, tx, src, dest, angle, center, flip);
}

void Mapping::map_render(SDL_Renderer* r)
{
	//ints to index through window ehight and width
	int x = 0;
	int y = 0;
	//loop through the 2d vector tiles
	for (std::size_t i = 0; i < mTiles.size(); ++i)
	{
		for (std::size_t j = 0; j < mTiles[i].size(); j++)
		{

			TileType tile = mTiles[i][j];

			//multiply the tile size by the indices to spread the tiles consequtively
			draw_rect.x = tile_size * j;
			draw_rect.y = tile_size * i;

			//switch on the tiles, draw what that tile is
			switch (tile)
			{
			case Empty:
				SDL_RenderCopy(r, emp_tx, NULL, &draw_rect);
				break;
			case Block:
				SDL_RenderCopy(r, blk_tx, NULL, &draw_rect);
				break;
			case Wild:
				SDL_RenderCopy(r, wld_tx, NULL, &draw_rect);
				break;
			default:
				throw("No valid tile passed");
					break;
			}
		}
	}
	//render the player sprite
	render_ex(r, sprite_tx, &src_sprite_rect, &sprite_rect, flipper);
}
//funntion to remove a row as it leaves the players view
void Mapping::move_row_offscreen(bool upward)
{
	//index i is how many tiles that will be needed in a single column
	//this should be a constant probably
	int i = screenWidth / tile_size + 1;
	//random int 
	int random_int;

	//row for inserting into front of 
	std::vector<TileType> row;

	//loop through the row size and get a random number
	//a new row will be needed regardless of the direction
	for (; i > 0; i--)
	{

		//rand generator
		random_int = rand() % 100;

		//percent ranges determine what tile gets pushed to the row vector
		//empty should be the most prevalent
		if (random_int <= 50)
			row.push_back(Empty);
		if (random_int > 50 && random_int <= 60)
			row.push_back(Block);
		if (random_int > 60)
			row.push_back(Wild);
	}
	//make a wall on the left and right sides
	row[0] = Block;
	row[row.size() - 1] = Block;
	row[row.size() - 2] = Block;

	//both of these should keep the capacity from reallocating because they reduce the size first then add an element
	//if upward pop the back and insert the new row
	if (upward)
	{
		//Remove the bottom row 
		mTiles.pop_back();
		//insert the new row at the front
		mTiles.insert(mTiles.begin(), row);

	}
	//if downward, remove the first row and push the new row to the back
	else
	{
		//remove the first row via an erase
		mTiles.erase(mTiles.begin());
		//push new row to the back
		mTiles.push_back(row);

	}
}

void Mapping::move_column_offscreen(bool leftward)
{
	//keep the previous movement int so we have something to compare to 
	previous_lr_movement = left_right_movement;
	//row for inserting into front of 
	std::vector<TileType> column = std::vector<TileType>();

	//iterator
	int i = 0;
	//loop through the map
	for (auto vec : mTiles)
	{
		//take the first or last element of each push to column
		if (!leftward)
		{
			//make moving right increments
			left_right_movement++;

			//save the column element
			column.push_back(vec[0]);
			//erase that element from the map;
			//hopefully erase will resize the vector automatically
			mTiles[i].erase(mTiles[i].begin());

			//if the abs value decreased then you know the player changed direction
			if (std::abs(left_right_movement) < std::abs(previous_lr_movement) && !storage.empty() )
			{
				//insert from last vector in storage into the last column of map
				mTiles[i].insert(mTiles[i].end(), storage.back()[i]);	
			}
			//else insert a block at the front if the player changed direction
			else
				mTiles[i].insert(mTiles[i].end(), Block);
		}	
		else
		{
			//make moving left decrements
			left_right_movement--;

			//save the column element
			column.push_back(vec[vec.size() - 1]);
			//erase that element from the map;
			//hopefully erase will resize the vector automatically
			mTiles[i].pop_back();
			//we don't want to place blocks at the front if the chracterhas'nt returned to the middle of lane

			if (std::abs(left_right_movement) < std::abs(previous_lr_movement) && !storage.empty())
			{
				//insert from the last member in storage into the map's front
				mTiles[i].insert(mTiles[i].begin(), storage.back()[i]);
			}
			//else insert a block at the front if the player changed direction
			else
				mTiles[i].insert(mTiles[i].begin(), Block);

		}
		//increment
		i++;
	}

	//if the conditions to replace a row from storage were met then pop storage
	if(std::abs(left_right_movement) < std::abs(previous_lr_movement) && !storage.empty())
		storage.pop_back();

	else
	{
		//save the column in storage
		storage.push_back(column);
		//clear the column
		column.clear();
	}
		
}

void Mapping::move_sprite_lr(bool leftward)
{
	if(leftward)
		//if(sprite_rect.x >= tile_size)
			sprite_rect.x -= tile_size;
	else
		//if (sprite_rect.x <= screenWidth - tile_size)
			sprite_rect.x += tile_size;

	std::cout << sprite_rect.x << std::endl;
}



//function to save map to close game and open it later
void Mapping::SaveMap()
{
	//create a text file with the name of this string
	std::ofstream map_save("savedat/mapsave.mp");
	//give it a title
	map_save << "MAP" << std::endl;
	//loop through every element in mTiles
	for (std::size_t i = 0; i < mTiles.size(); ++i)
	{
		for (std::size_t j = 0; j < mTiles[i].size(); j++)
		{
			TileType tile = mTiles[i][j];
			//write the tile
			map_save << tile << ",";
		}
		//end rows with a $
		map_save << "$"<< std::endl;
	}
	map_save.close();
}

//function to load from the saved map
void Mapping::ReadMap()
{
	//open file
	std::ifstream infile("SaveData/mapsave.mp");

	//iterator to read each chracter in the file
	char iterate;
	//$ signifies a new row
	char end_sign = '$';
	//indexers
	std::size_t i = 0;
	std::size_t j = 0;

	//this loop will read every character in the file
	//It runs pretty slowly, reading each character might not be the best way
	//or maybe the library function is just slow
	while (infile >> iterate)
	{
		//increment the outer index if the inner reaches the size of the first vector
		if (j >= mTiles[0].size())
		{
			j = 0;
			i++;
		}
		//when it reaches capacity just end it.
		if (i >= mTiles.size())
		{
			break;
		}
		//switch on every character, then increment the innner index
			switch (iterate)
			{
			case '0':
				mTiles[i][j] = Empty;
				j++;
				break;
			case '1':
				mTiles[i][j] = Wild;
				j++;
				break;
			case '2':
				mTiles[i][j] = Block;
				j++;
				break;
			default:
				break;
			}
	}
	//close the file and exit function
	infile.close();
}