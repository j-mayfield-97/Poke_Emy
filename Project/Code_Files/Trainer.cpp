#pragma once

#include "Trainer.h"

//trainer and mon should probably inherent from common a "combatant" class


Trainer::~Trainer()
{
}

//give complete stats for dev purposeses
void Trainer::print_stats()
{
	std::cout << "Name: "<< this->name << std::endl;
	std::cout << "Attack: " << this->attack << std::endl;
	std::cout << "Defense: " << this->defense << std::endl;
	std::cout << "HP Total: " << this->hp << std::endl;
	//std::cout << "ID: " << this->type_ID << std::endl;

	//prints all relavent info for all mons 
	for (Mon* i : mons)
	{
		i->printStats();
	}
}

void Trainer::catch_mon(Mon* m)
{
	m->alive_check();
	if (m->catch_mon())
	{
		//add the mon to the collection
		mons.push_back(m);
		
	}
	else 
	{
		std::cout << "couldn't catch mon" << std::endl;
	}
}


//just use to test if catching works
void Trainer::make_catchable(Mon* m)
{
	m->make_catchable();
}

void Trainer::hp_increase(Mon* m)
{
	m->hp_increase();
}

// A random pokemon chosen then add that pokemon 
void Trainer::random_encounter()
{
	//transition animation
	
	//screenclear

	//get the mons to display
	
}
