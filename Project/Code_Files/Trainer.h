#pragma once
#include <vector>
#include "Mon.h"

class Trainer: public Combatant
{

public:
	Trainer(const char* _name, int att, int def, int _health) :
		Combatant(_name, att, def, _health) 
	{ //the player never starts in a battle
		battling = false;
	}
	~Trainer();

	//tiles moved in one direction makes the game more challenging and rewarding
	int one_direction;

	std::vector<Mon*> mons;

	void print_stats();
	void catch_mon(Mon* m);
	void make_catchable(Mon* m);
	void hp_increase(Mon* m);
	void random_encounter();

	//work the way select_rect works for battle menu
	//to be set externally by menu options
	Combatant* selected_target;

	//bool to keep the player in a battle
	bool battling;

};

