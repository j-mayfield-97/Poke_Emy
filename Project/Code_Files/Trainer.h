#pragma once
#include <vector>
#include "Mon.h"

class Trainer
{
public:
	Trainer();
	~Trainer();

	const char* name;
	int type_ID;
	int attack;
	int defense;
	int health_pool;
	std::vector<Mon*> mons;
	Mon* monsters[3];

	void catch_mon(Mon* m);
	void attack_mon(Mon* m);
	void health_reduction(Mon* m);
};

