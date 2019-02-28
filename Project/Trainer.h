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
	Trainer(const char* file_name);
	~Trainer();

	//tiles moved in one direction makes the game more challenging and rewarding
	int floors;

	//this has got to be made private later!!
	std::vector<Mon*> mons;

	void write_stats();
	void read_stats();
	void print_stats();
	void add_mon(Mon* m);
	void catch_mon(Mon* m);
	void make_catchable(Mon* m);
	void hp_increase(Mon* m);
	void random_encounter();
	Mon* mon_returner();
	void mon_cycler();

	//work the way select_rect works for battle menu
	//to be set externally by menu options
	Combatant* selected_target;

	//bool to keep the player in a battle
	bool battling;

	Trainer* opponent_t;
};

