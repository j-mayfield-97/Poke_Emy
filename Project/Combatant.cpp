#include "Combatant.h"



Combatant::Combatant()
{
}

Combatant::Combatant(const char * _name, int att, int def, int _health)
{
	name = _name;
	attack = att;
	defense = def;
	hp = max_health = _health;
	alive = true;
}


Combatant::~Combatant()
{
}

//mshould just have move effcts impliment from json
void Combatant::attack_creature(Moves mov)
{
	//deatch check
	if (!alive_check())
		return;
	
	if (opponent->alive)
	{
		//run effect
		//place holder math
		int damage = (int)((mov.baseAtt + this->attack) / opponent->defense);
		opponent->hp = opponent->hp - damage;
		std::cout << "hit for " << damage << std::endl;
		std::cout << opponent->name << " current health is " << opponent->hp << std::endl;
	}
	else
		std::cout << "opponent " << opponent->name << "is dead" << std::endl;
}

void Combatant::learnMoves(Moves mov)
{

	//deatch check
	if (!alive_check())
		return;

	move_list.push_back(mov);

	std::cout << name << " learned " << mov.name_move << std::endl;
}

void Combatant::set_opponent(Combatant * target)
{
	//deatch check
	if (!alive_check())
		return;

	opponent = target;
}

//the death of a true monster
bool Combatant::alive_check()
{
	if (hp < 0)
	{
		alive = false;
		std::cout << name << " died, they can't act" << std::endl;
		return alive;
	}
	else
		std::cout << name << " still alive." << std::endl;
	return alive;
}