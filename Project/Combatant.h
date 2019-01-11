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

	void attack_creature(Moves mov);
	void learnMoves(Moves mov);
	void set_opponent(Combatant* target);

	bool alive_check();

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

	//Mon* monsters[3];
	Combatant* opponent;
	std::vector<Moves> move_list;
};

