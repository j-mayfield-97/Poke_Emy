#pragma once
#include <vector>
#include "Mon.h"
#include "../Items.h"

//make all the values private!!!!
class Trainer: public Combatant
{
	
	//game gets more challenging as floors rise
	int floors;
	//coins to be spent at shops
	int trainer_coins;
public:
	Trainer(const char* _name, int att, int def, int _health) :
		Combatant(_name, att, def, _health) 
	{ //the player never starts in a battle
		battling = false;
		floors = 1;
	}
	Trainer(const char* file_name);
	~Trainer();

	//this has got to be made private later!!
	std::vector<Mon*> mons;

	//trainer item storage limited to 10 for now
	std::vector<Items> trainer_items;

	//abilities or moves names
	const char* t_ability_path1 = "assets/Yino.png";
	const char* t_ability_path2 = "assets/Yino.png";
	const char* t_ability_path3 = "assets/Yino.png";
	const char* t_ability_path4 = "assets/Yino.png";

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
	void add_item(Items* item_c);
	int increment_floor(int inc);
	int coin_helper(int val);

	//
	void(*player_ability(int index))(Combatant*, Combatant*, int);


	//work the way select_rect works for battle menu
	//to be set externally by menu options
	Combatant* selected_target;

	//bool to keep the player in a battle
	bool battling;

	//keep the opponent trainer
	Trainer* opponent_t;
};

