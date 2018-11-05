#pragma once
#include <vector>
#include "Moves.h"//would the mon need to know if its caught and who its owner is?

class Mon
{
public: 
	Mon();
	~Mon();

	const char* name;
	bool alive = true; //i think bools are intialized to true automatically? if dead it shouldnt be able to do anything.
	int type1; //tags for type ie reg = 0 , love = 1
	int type2;
	const char* outBattlePath;//have these paths to sprite component when needed
	const char* inBattlePath;

	int attack;
	int defense = 5;
	//to be implemented later
//	int magic;
//	int points; //points from battle and points to use magic
	int originalHp = 25;
	int hp = 25;
	int pp;//points //in battle

	//this array is probably better than a vector
	//four moves max for now
	//blank moves array
	Moves moves[4] = {NULL,NULL,NULL,NULL};

	void learnMoves(Moves mov)
	{
		int i = 0;
		//loop through the array and iff the array has a null slot fill in the new move
		for (auto mIndex : moves)
		{
			if (moves[i] == NULL)
			{
				moves[i] = mov;
				std::cout << name << " learned " << mov.name_move << std::endl;
				return;
			}
		}
		//if the loop doesn't find any empty slots then it should reset
		std::cout << name << " could not learn " << mov.name_move << std::endl;
	}

	//these print funtions should push sdl text to the screen instead of printing to console
	void printMoves()
	{
		std::cout << name << " knows ";
		for (auto i : moves)
		{
			std::cout << i.name_move << ", ";
		}
		std::cout << std::endl;
	}

	void printStats()
	{
		std::cout << name << " Stats "<<std::endl;

			std::cout <<" def is " << defense<< std::endl;
			std::cout << " hp is " << hp << std::endl;
			death();
			printMoves();
	}

	//the death of a true monster
	void death()
	{
		if (hp < 0)
		{
			alive = false;
			std::cout << name << " died" << std::endl;
		}
		else
			std::cout << name << " still alive" << std::endl; 
	}
};