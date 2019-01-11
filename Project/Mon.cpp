#pragma once

#include "Code_Files/Mon.h"

Mon::Mon()
{
	//init an empty mon, to later be filled in with a separate function
}

Mon::~Mon()
{

}

bool Mon::catch_mon()
{
	//make sure its alive
	alive_check();

	if (alive && hp < (max_health / 2))
	{
		//catch condition success
		std::cout << "caught " << name << std::endl;
		return true;
	}
	else
	{
		//failure
		return false;
	}
}


//just use to test if catching works
void Mon::make_catchable()
{
	hp = (hp / 2) - 1;
	std::cout << name << "has been made catchable" << std::endl;
}

void Mon::hp_increase()
{
	hp += 10;
	std::cout << name << " hp increases " << hp << "/" << max_health << std::endl;
}

//these print funtions should push sdl text to the screen instead of printing to console
void Mon::printMoves()
{

	std::cout << name << " knows ";
	for (auto i : move_list)
	{
		std::cout << i.name_move << ", ";
	}
	std::cout << std::endl;
}

void Mon::printStats()
{
	std::cout << name << " Stats " << std::endl;

	std::cout << " def is " << defense << std::endl;
	std::cout << " hp is " << hp << std::endl;
	alive_check();
	printMoves();
}

