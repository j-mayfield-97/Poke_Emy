#include "Combatant.h"
#include <time.h>


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

//i'm realizing now that stat is no longer necessary as the rewritting the function made all members of both combatatnts accessible
void Combatant::basic_attack(Combatant* actor, Combatant* com, int stat)
{
	int damage = std::ceil((actor->attack) / com->defense);
	com->hp = com->hp - damage;
	std::cout << actor->name << " hit "<< com->name <<" for " << damage << std::endl;
	std::cout << com->name << " current health is " << com->hp << std::endl;
}

void Combatant::basic_defense(Combatant* actor, Combatant* com, int stat)
{
	int d = std::ceil( actor->defense / 10);
	com->defense += d;
	std::cout << actor->name << " raised defense " << com->hp << std::endl;
}

void Combatant::lick_wounds(Combatant* actor, Combatant* com, int stat)
{
	com->hp += (actor->defense / 20);
	std::cout << actor->name << " licked "<< com->name << "'s wounds. Thier hp is" << com->hp << " out of "<<  com->max_health << std::endl;
}

void Combatant::learnMoves(Moves mov)
{

	//deatch check
	if (!alive_check())
		return;

	move_list.push_back(mov);

	std::cout << name << " learned " << mov.name_move << std::endl;
}

void Combatant::set_opponent(Combatant* target)
{
	//deatch check and null pointer checks
	if (!alive_check() || (target == nullptr || target == NULL))
		return;
	else
		opponent = target;

	std::cout << "opponent is now " << target->name << std::endl;
}

Combatant* Combatant::get_opponent()
{

	if (opponent != nullptr & opponent != NULL)
	{
		return opponent;
	}
	//throw();
	return nullptr;
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

int Combatant::set_ID()
{
	//uuid(lua) will be used in the future
	//in its current form it's me struggling with the rand() functions
	srand(time(NULL));
	int a = rand();
	srand(time(NULL));

	if (a % 2 == 1)
		a += rand();
	srand(time(NULL));

	long int t = (long int)time(NULL);

	//going to need a better way to error handle in the future
	if (t >= _MAX_INT_DIG)
		_ASSERT("random int generator failed", NULL);

	call_ID = rand() + a + t;
	return call_ID;
}

//function for setting id to help with grabbing data from a json read
int Combatant::set_ID(int id)
{
	call_ID = id;
	return call_ID;
}

int Combatant::get_ID()
{
	return call_ID;
}

