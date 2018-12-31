#pragma once

#include "Trainer.h"

//trainer and mon should probably inherent from common a "combatant" class

Trainer::Trainer()
{
	//init values
	name = "null name trainer ";
	attack = 14;
	one_direction = 0;
	opponent_mon = NULL;
	//start with an empty set of mons
	//monsters[0] = NULL;
	//monsters[1] = NULL;
	//monsters[2] = NULL;

	//testin mon, you'll usually start without one
	Mon* test_mon = new Mon();
	mons.push_back(test_mon);
	
	//the player never starts in a battle
	battling = false;
}


Trainer::~Trainer()
{
}

//give complete stats for dev purposeses
void Trainer::print_stats()
{
	std::cout << "Name: "<< this->name << std::endl;
	std::cout << "Attack: " << this->attack << std::endl;
	//std::cout << "Defense: " << this->defense << std::endl;
	//std::cout << "HP Total: " << this->health_pool << std::endl;
	//std::cout << "ID: " << this->type_ID << std::endl;

	//prints all relavent info for all mons 
	for (Mon* i : mons)
	{
		i->printStats();
	}
}

void Trainer::catch_mon(Mon* m)
{
	if (m->alive && m->hp < (m->originalHp / 2 ))
	{
		mons.push_back(m);
		std::cout << "caught "<< m->name << std::endl;
	}
	else 
	{
		std::cout << "couldn't catch mon" << std::endl;
	}
}

void Trainer::attack_mon(Mon* m)
{
	std::cout << m->name << " current health is " << m->hp<<std::endl;
	if (m->alive)
	{
		int projDamage = (int)(attack / m->defense);
		m->hp = m->hp - projDamage;
		std::cout << "hit for "<< projDamage << std::endl;
	}
	else
	{
		m->hp = m->hp - attack;
		std::cout << "corpse hit for " << m->hp << "/" << m->originalHp << std::endl;

	}
}

//just use to test if catching works
void Trainer::make_catchable(Mon* m)
{
	m->hp = (m->hp / 2) - 1;
	std::cout << m->name << "has been made catchable" << std::endl;
}

void Trainer::hp_increase(Mon * m)
{
	m->hp += 10;
	std::cout << m->name <<" hp increases " << m->hp << "/" << m->originalHp << std::endl;
}

// A random pokemon chosen then add that pokemon 
void Trainer::random_encounter()
{
	//transition animation
	
	//screenclear

	//get the mons to display
	
}
