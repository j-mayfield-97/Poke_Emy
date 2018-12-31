#pragma once
#include <vector>
#include "Mon.h"

class Trainer
{
	const char* name;
	int type_ID;
	int attack;
	int defense;
	int health_pool;
	
	//Mon* monsters[3];
	Mon* opponent_mon;

public:
	Trainer();
	~Trainer();

	std::vector<Mon*> mons;

	//tiles moved in one direction makes the game more challenging and rewarding
	int one_direction;

	void print_stats();
	void catch_mon(Mon* m);
	void attack_mon(Mon* m);
	void make_catchable(Mon* m);
	void hp_increase(Mon* m);
	void random_encounter();

	//bool to keep the player in a battle
	bool battling;

};

