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
	~Combatant();

	static void basic_attack(Combatant* actor, Combatant* com, int stat);
	static void basic_defense(Combatant* actor, Combatant* com, int stat);
	static void lick_wounds(Combatant* actor, Combatant* com, int stat);

	//i'm currently pivoting away from how these work
	void attack_creature(Moves mov);
	void learnMoves(Moves mov);

	//make all set_ virtual
	void set_opponent(Combatant* target);
	Combatant* get_opponent();

	bool alive_check();

	//get and set for creating IDs and returning them
	//the way these run is temporary
	int set_ID();
	int set_ID(int id);
	int get_ID();

	//this needs to be private later
	std::vector<Moves> move_list;

	// Pure Virtual Function 
	//defined in mons and trainer
	virtual void write_stats() = 0;
	virtual void read_stats() = 0;

	const char* outBattlePath = "EMPTY.PNG";
	const char* inBattlePath = "EMPTY.PNG";

protected:

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

	//Mon* monsters[3];
	Combatant* opponent;
	
};

