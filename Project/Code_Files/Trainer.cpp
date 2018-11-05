#pragma once

#include "Trainer.h"



Trainer::Trainer()
{
	//init values
	attack = 14;
	//start with an empty set of mons
	monsters[0] = NULL;
	monsters[1] = NULL;
	monsters[2] = NULL;
}


Trainer::~Trainer()
{
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
void Trainer::health_reduction(Mon* m)//just use to test if catching works
{
	m->hp = (m->hp / 2) - 1;
}