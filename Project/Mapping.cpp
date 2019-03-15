#pragma once

#include "Mapping.h"
#include <random>
#include <ctime>
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
	emp_tx = IMG_LoadTexture(r, "assets/wall32.png");
	blk_tx = IMG_LoadTexture(r, "assets/water32.png");
	wld_tx = IMG_LoadTexture(r, "assets/dirt32.png");
	dor_tx = IMG_LoadTexture(r, "assets/grass32.png");
	pnk_tx = IMG_LoadTexture(r, "assets/pnk.png");
}

//clean up 
Mapping::~Mapping()
{
	SDL_DestroyTexture(emp_tx);
	SDL_DestroyTexture(blk_tx);
	SDL_DestroyTexture(wld_tx);
	SDL_DestroyTexture(dor_tx);
	SDL_DestroyTexture(pnk_tx);
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

//return the items to be added to trainer
//despawn them by erasing them from vector
Items* Mapping::item_collision()
{
	Items* item_c = nullptr;

	//loop through item vec then have the object that shares the players rect be erased
	for (int i = 0; i < items_vec.size(); i++)
	{
		if (items_vec[i]->irect.x == sprite_rect.x && items_vec[i]->irect.y == sprite_rect.y)
		{
			item_c = items_vec[i];
			items_vec.erase(items_vec.begin() + i); 
		}
	}
	return item_c;
}

bool immeadiate_collsion()
{
	SDL_assert("nope not ready yet");
	return false;
}

//You should have this function retruning a row instead of pushing it back in the function, it'd be more useful that way
void Mapping::random_tiles_row()
{
	SDL_assert("nope not ready yet");
}
//function to place single door for egress make sure it is rendered last
void Mapping::place_door()
{
	int x;
	int y;
	//rand generator
	x = rand() % (mTiles[0].size() - 4);
	y = rand() % (mTiles.size() - 4);
	//place door
	mTiles[x + 1][y + 1] = Door;
}

void Mapping::place_trainers()
{
}

//this fuctionos gunna get very large, consider moving it
void Mapping::place_items(int row, int col, SDL_Renderer* r)
{
	//to be replaced by an item enum
	//iEmpty is used to hold space on the overworld
	ItemType item_id = iEmpty;
	
	//get a rand in to decide which items are placed and loaded
	//texture to be added to vec
	SDL_Texture* _tx;
	const char* path = "";

	bool animation = false;

	//constants for animation to be assigned in if statements
	int w = -1;
	int h = -1;
	int offsetX = -1;
	int offsetY = -1;
	int frames = -1;
	int roww = -1;
	int column = -1;
	int sprites_in_row = -1;
	int delay = -1;
	int c_val = 0;

	int rand_int = rand() % 150;
	if (rand_int < 30)
	{
		item_id = iMon;
		path = "assets/floggy.png";
	}
	if (rand_int >= 50 && rand_int < 100)
	{
		item_id = iHeld;
		path = "assets/ball.png";
	}
	if (rand_int >= 100 && rand_int < 120)
	{
		item_id = iCoin;
		path = "assets/coin_sacio.png";

		//coin value between 1 and 21 
		c_val = rand_int - 99;

		 w = 16;
		 h = 16;
		 offsetX = 0;
		 offsetY = 0;
		 frames = 12;
		 roww = 0;
		 column = 0;
		 sprites_in_row = 12;
		 delay = 100;

		 animation = true;
	}
	if (rand_int >= 120 && rand_int < 150)
	{
		item_id = iLife;
		path = "assets/sSoul_patvanmackelberg.png";
		w = 16;
		h = 16;
		offsetX = 0;
		offsetY = 0;
		frames = 4;
		roww = 0;
		column = 0;
		sprites_in_row = 4;
		delay = 100;

		animation = true;
	}

	_tx = IMG_LoadTexture(r, path);
	//create item obj to be pushed to vec
	Items* item_c = new Items(row, col, item_id, _tx);

	//set constants for animations if its an animation
	if(animation)
		item_c->Set_Animation_Constants( w,  h,  offsetX,  offsetY,  frames,  roww,  column,  sprites_in_row,  delay);
	//get the width and height from sprite the x,y will be filled in render loop
	item_c->irect = sprite_rect;
	//set coin value
	item_c->value = c_val;
	//push
	items_vec.push_back(item_c);	
}

//the form of theis function is non optimal
//decouple render and placement 
// form a vetor of items
//change the intial mapping loops to get the places of events 
void Mapping::render_items(SDL_Renderer* r)
{
	//loop through vec placing the rect at the coords and use the same index to get the texture for rendering
	for (auto item_c : items_vec)
	{
		item_c->irect.x = item_c->ix * tile_size;
		item_c->irect.y = item_c->iy * tile_size;

		if (item_c->constants_set)
			item_c->animate_help(r);
		else
			item_c->simple_render(r);
	}
}

//I've MESSED WITH the iteration and indexing in this function too much
//items spawning in ocean are no longer bugs they are features, will add an item to get items that spawn in water.
//Make a full map to start with
void Mapping::Form_Initial_Map( SDL_Renderer* r)
{
	//clear previous map
	mTiles.clear();
	items_vec.clear();
	//interator i is how many tiles that will be needed in a single column
	//this should be a constant
	int col_size = screenHeight / tile_size;

	int row_size = screenWidth / tile_size;

	int wall_size = row_size;

	//random int to decide what goes in tiles 
	int random_int;

	//tile to be pushed to row vector
	TileType to_push;

	//column array to keep tiles
	std::vector<TileType> row;

	//column array to keep tiles
	std::vector<TileType> wall_row;

	//make and push a wall row to the bottom of map
	for (int row_index = 0; row_index < row_size; row_index++)
		wall_row.push_back(Block);
	mTiles.push_back(wall_row);

	//seed for rand int
	srand(std::time(NULL));

	//loop through height of window and add randomized rows. kep in mind the actual 0th and final row and columns are going to be walls
	for (int col_index = 1; col_index < col_size; col_index++)
	{
		for (int row_index = -1; row_index != row_size; row_index++)
		{
			//defualt is empty
			to_push = Empty;
			//rand generator
			random_int = rand() % 100;
			//percent ranges determine what tile gets pushed to the row vector
			if (random_int > 50 && random_int <= 65)
				to_push = Block;
			if (random_int > 60 && random_int <= 75  /*embrace the bugs && !( col_index == 0 || row_index == -1 || col_index == col_size-1 || row_index == row_size-1)*/)
			{
				to_push = Event;
				place_items(row_index + 1, col_index, r);
			}
			if (random_int > 75)
				to_push = Wild;
			row.push_back(to_push);
		}
		//make a wall on the left top and bottom and right sides 
		row[0] = Block;
		row.pop_back();
		row[row.size() - 1] = Block;
	
		//push that new row to the map
		mTiles.push_back(row);

		row.clear();
	}
	mTiles.pop_back();
	//push wall to top of map
	mTiles.push_back(wall_row);

	int sprite_row = sprite_rect.y / tile_size;
	int sprite_column = sprite_rect.x / tile_size;
	//mkae sure player is not locked in , and has a way out
	//change tile under player
	mTiles[sprite_row][sprite_column] = Empty;
	//place door in map
	place_door();
}

//move thorugh sprite sheet to give the illusion of animations only to the player trainer
void Mapping::animate_player_help(int w, int h, int offsetX, int offsetY, int frames, int row, int column, int sprites_in_row, int delay)
{
	//w and h are size of sprites on the sheet in pixels
	//offsets from topleft in pixels
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
			case Event:
				SDL_RenderCopy(r, pnk_tx, NULL, &draw_rect);
				break;
			case Wild:
				SDL_RenderCopy(r, wld_tx, NULL, &draw_rect);
				break;
			case Door:
				SDL_RenderCopy(r, dor_tx, NULL, &draw_rect);
				break;
			default:
				throw("No valid tile passed");
					break;
			}
		}
	}
	//render and place items
	render_items(r);

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

}

void Mapping::move_sprite_ud(bool upward)
{
	if (upward)
		//if(sprite_rect.x >= tile_size)
		sprite_rect.y -= tile_size;
	else
		//if (sprite_rect.x <= screenWidth - tile_size)
		sprite_rect.y += tile_size;

}

//function to save map to close game and open it later
void Mapping::SaveMap()
{
	//create a text file with the name of this string
	std::ofstream map_save("SaveData/mapsave.mp");
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
			case '3':
				mTiles[i][j] = Door;
				j++;
				break;
			case '4':
				mTiles[i][j] = Event;
				j++;
				break;
			default:
				break;
			}
	}
	//close the file and exit function
	infile.close();
}