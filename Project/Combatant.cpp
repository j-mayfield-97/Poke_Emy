#pragma once

//need this to use itoa without the deprecated warning
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE  

#include "Combatant.h"
#include <time.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream> 
#include <string.h>
#include <string>


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

Combatant::Combatant(const char* file_name)
{
	//concat the trainer name into the save file name
	std::string path_prefix = "SaveData/";
	std::string extension = ".json";

	std::string truepath = path_prefix + file_name + extension;

	//read
	std::ifstream ifs(truepath);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;

	if (doc.ParseStream(isw).HasParseError())
	{
		ifs.close();
		std::cout << rapidjson::GetParseErrorFunc();
		throw(rapidjson::GetParseErrorFunc());
	}
	ifs.close();

	//access members then read them
	//and set the object to the values read

	//a conversion to string and back to const char* is necessary, rapidjson is deleting the string upon exiting function
	rapidjson::Value& val_name = doc["Name"];
	const char* _name = val_name.GetString();
	_help_name = _name;
	this->name = _help_name.c_str();

	rapidjson::Value& val_att = doc["Attack"];
	this->attack = val_att.GetInt();

	rapidjson::Value& val_def = doc["Defense"];
	this->defense = val_def.GetInt();

	rapidjson::Value& val_hpt = doc["HpTotal"];
	this->max_health = val_hpt.GetInt();

	rapidjson::Value& val_hp = doc["Hp"];
	this->hp = val_hp.GetInt();

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
	int damage = std::ceil((actor->attack));
	com->hp = com->hp - damage;
	std::cout << actor->name <<  " hit "<< com->name << " for " << damage << std::endl;
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
	com->hp += std::ceil(actor->defense / 20);
	std::cout << actor->name <<  " licked "<< com->name <<  "'s wounds. Thier hp is" << com->hp << " out of "<<  com->max_health << std::endl;
}

void Combatant::learnMoves(Moves mov)
{
	//deatch check
	if (!alive_check())
		return;

	move_list.push_back(mov);

	std::cout << name << " learned " << mov.name_move << std::endl;
}

void Combatant::set_current_target(Combatant* target)
{
	//deatch check and null pointer checks
	if (!alive_check() || (target == nullptr || target == NULL))
		return;
	else
		opponent = target;

	std::cout << "opponent is now " << opponent->name_get() << std::endl;
}

Combatant* Combatant::get_opponent()
{

	if (opponent != nullptr && opponent != NULL)
	{
		std::cout << "opponent is " << opponent->name_get();
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

//simple name getter
const char * Combatant::name_get()
{
	return name;
}

//return the "hp / max_health" as a string for the battlemenu to turn into text
const char * Combatant::health_string()
{
	//there has to be a better way to do this
	char buffer[50];
	char buffer2[50];
	const char* hp_str = itoa(hp, buffer, 10);
	const char* max_str = itoa(max_health, buffer2, 10);
	char str[80];
	strcpy(str, hp_str);
	strcat(str, "/");
	strcat(str, max_str);
	return str;
}

int Combatant::get_hp()
{
	return hp;
}

int Combatant::get_max_hp()
{
	return max_health;
}

//switch on move id provided by mon or trainer then return function pointer
void(*Combatant::moves_returner(int mvid))(Combatant *, Combatant *, int)
{

	switch (mvid)
	{
	case 0:
		return basic_attack;
		break;
	case 1:
		return basic_defense;
		break;
	case 2:
		return lick_wounds;
		break;
	default:
		throw("not a move");
		break;
	}
	return nullptr;
}

