#pragma once
#include <vector>
#include "Code_Files/Moves.h"

class Combatant
{
	//how does this know what trainer is?
	// does it make sense to have a derived class also be a friend class
public:
	Combatant();
	Combatant(const char* _name, int att, int def, int _health);
	Combatant(const char* file_name);
	~Combatant();

	static void basic_attack(Combatant* actor, Combatant* com, int stat);
	static void basic_defense(Combatant* actor, Combatant* com, int stat);
	static void lick_wounds(Combatant* actor, Combatant* com, int stat);

	//i'm currently pivoting away from how these work
	void attack_creature(Moves mov);
	void learnMoves(Moves mov);

	//make all set_ virtual
	//rewrite these to target 
	void set_current_target(Combatant* target);
	Combatant* get_opponent();

	bool alive_check();

	//get and set for creating IDs and returning them
	//the way these run is temporary
	int set_ID();
	int set_ID(int id);
	int get_ID();

	//this needs to be private later
	std::vector<Moves> move_list;

	// Pure Virtual Functions
	//defined in mons and trainer
	virtual void write_stats() = 0;
	virtual void read_stats() = 0;

	const char* outBattlePath = "EMPTY.PNG";
	//temp testing sprite path
	const char* inBattlePath = "assets/ufoo.png";
	const char* name_get();
	const char* health_string();

	int get_hp();
	int get_max_hp();

protected:

	std::string _help_name;

	const char* name;
	bool alive;
	int type_ID;
	int attack;
	int defense;
	int max_health;
	int hp;
	
	//will use uuid in the future
	//for now rand int for id
	int call_ID;

	Combatant* opponent;

	void(*moves_returner(int index))(Combatant*, Combatant*, int);	
};

