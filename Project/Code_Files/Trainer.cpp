#pragma once
//need this to use itoa without the deprecated warning
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE  

#include "Trainer.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream> 
#include <string.h>
#include <string>
#include <cstring>

//Constructor that loads from file
Trainer::Trainer(const char* file_name) : Combatant(file_name)
{
	//concat the trainer name into the save file name
	std::string path_prefix = "SaveData/";
	std::string extension = ".json";

	std::string truepath = path_prefix + file_name + extension;

	//read
	std::ifstream ifs(truepath);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	ifs.close();

	rapidjson::Value& val_mon_list = doc["mon_list"];

	//go through mon_list array and have add_mon() push them to the mons vector

	for (rapidjson::SizeType i = 0; i < val_mon_list.Size(); i++) // Uses SizeType instead of size_t
	{
		int digits = val_mon_list[i].GetInt();

		//convert uuid to a string
		char buffer[50];
		const char* iidd = itoa(digits, buffer, 10);
		char path[80];
		strcpy(path, "Mons/m");
		strcat(path, iidd);

		Mon* mn = new Mon(path);

		this->add_mon(mn);

	}
	//LOAD  floors from json!!!
	floors = 1;
	//trainers don't start in battles
	battling = false;
}

Trainer::~Trainer()
{
}

//read from jsons to fill in the stats
void Trainer::read_stats()
{
	//concat the trainer name into the save file name
	std::string path_prefix = "SaveData/Trainer/";
	std::string extension = ".json";

	std::string truepath = path_prefix + name + extension;

	//read
	std::ifstream ifs(truepath);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	ifs.close();

	//access members then read them
	//and set the object to the values read
	rapidjson::Value& val_name = doc["Name"];
	const char * temp ;
	temp = val_name.GetString();
	this->name = temp;

	rapidjson::Value& val_att = doc["Attack"];
	this->attack = val_att.GetInt();

	rapidjson::Value& val_def = doc["Defense"];
	this->defense = val_def.GetInt();

	rapidjson::Value& val_hpt = doc["HpTotal"];
	this->max_health = val_hpt.GetInt();

	rapidjson::Value& val_hp = doc["Hp"];
	this->hp = val_hp.GetInt();

	rapidjson::Value& val_mon_list = doc["mon_list"];

	Trainer * replace;

	//go through mon_list array and have add_mon() push them to the mons vector
	
	for (rapidjson::SizeType i = 0; i < val_mon_list.Size(); i++) // Uses SizeType instead of size_t
	{
		Mon* m = new Mon("n", -1, -1, -1);
		int digits = val_mon_list[i].GetInt();
		//att the list index return that int  then have read stats find the mon with that
		m->read_stats( digits );

		this->add_mon(m);

		delete m;
	}	
}

//write stats as saved data in a .txt
void Trainer::write_stats()
{
	//init json
	const char* json = "{\"Name\":\"nam\", \"Attack\":-1, \"Defense\":-1, \"HpTotal\":-1, \"Hp\":-1, \"sprite_path\":\"path\", \"mon_list\" : []}";

	//init doc to parse json
	rapidjson::Document doc;

	//jsonify the json string
	doc.Parse(json);

	//access members then set them
	rapidjson::Value& val_name = doc["Name"];
	val_name.SetString(rapidjson::StringRef(name));


	rapidjson::Value& val_att = doc["Attack"];
	val_att.SetInt(attack);

	rapidjson::Value& val_def = doc["Defense"];
	val_def.SetInt(defense);

	rapidjson::Value& val_hpt = doc["HpTotal"];
	val_hpt.SetInt(max_health);

	rapidjson::Value& val_hp = doc["Hp"];
	val_hp.SetInt(hp);

	//needs an allocator in order to push values to an array
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value& val_mon_list = doc["mon_list"];
	
	//go through the mons vector and write the mons ID to mon_list to be read later
	int index = 0;
	for (Mon* i : mons)
	{
		//rapidjson pushback
		val_mon_list.PushBack(i->get_ID(), allocator);
	}

	//concat the trainer name into the save file name
	std::string path = "SaveData/Trainer/";
	std::string extension = ".json";

	std::string truepath = path + name + extension;

	//open file, use the outstream wrapper to write to file
	std::ofstream ofs(truepath);
	rapidjson::OStreamWrapper osw(ofs);
	rapidjson::Writer<rapidjson::OStreamWrapper> writerwr(osw);
	doc.Accept(writerwr);

	//then close file
	ofs.close();

	//free the allocator memory
	allocator.Free(nullptr);
}

//give complete stats for dev purposeses
void Trainer::print_stats()
{
	std::cout << "Name: "<< this->name << std::endl;
	std::cout << "Attack: " << this->attack << std::endl;
	std::cout << "Defense: " << this->defense << std::endl;
	std::cout << "HP Total: " << this->hp << std::endl;
	//std::cout << "ID: " << this->type_ID << std::endl;


	//prints all relavent info for all mons 
	for (Mon* i : mons)
	{
		i->printStats();
	}
}

//should probably set it so that it writes as soon as it adds a mon
void Trainer::add_mon(Mon* m)
{
	if (mons.size() + 1 > 4)
	{
		std::cout << "cannot have more than 4 mons" << std::endl;
		return;
	}
	//make sure the mon pointer points to a mon and not a null
	if (m == nullptr || m == NULL)
	{
		std::cout << "mon non-existent" << std::endl;
		return;
	}	
	//search vector to make sure no duplicate is added
	if (std::find(mons.begin(), mons.end(), m) != mons.end())
	{
		std::cout << "mon already in party" << std::endl;
		return;
	}
	//add the mon to the collection if no error encountered
	mons.push_back(m);
}

void Trainer::catch_mon(Mon* m)
{
	m->alive_check();
	if (m->catch_mon())
	{
		//add the mon to the collection
		add_mon(m);
	}
	else 
	{
		std::cout << "couldn't catch mon" << std::endl;
	}
}


//just use to test if catching works
void Trainer::make_catchable(Mon* m)
{
	m->make_catchable();
}

void Trainer::hp_increase(Mon* m)
{
	m->hp_increase();
}

// A random mon chosen then add that mon 
void Trainer::random_encounter()
{
	//transition animation
	
	//screenclear

	//get the mons to display
	
}

Mon* Trainer::mon_returner()
{
	//throw an error
	if (mons.empty())
		return nullptr;
	else
		return mons.back();
}

void Trainer::mon_cycler()
{
	//throw error
	if (mons.empty())
	{
		throw("no mons to cycle");
		return;
	}

	//really simple vector rotation 
	//theres a function for this in std but it seems unecessary to bring it in
	Mon* hold;
	hold = mons.back();
	mons.pop_back();
	mons.insert(mons.begin(), hold);

	std::cout << mons.back()->name_get() << "currently selected mon" << std::endl;
}

//validate and add pick-up items to the item_vector
void Trainer::add_item(Items * item_c)
{
	Items addition = *item_c;
	switch (item_c->iid)
	{
	case iCoin:
		coin_helper(item_c->value);
		item_c->value = 0;
		std::cout << "coins total : " << trainer_coins << std::endl;
		break;
	case iHeld:
		//certain items shouldnt have duplicates
		//in which case loop through item vec and check if its already in then 
		std::cout << "held item added" << std::endl;
		//for right now just push it the same
		
		trainer_items.push_back(addition);
		break;
	case iLife:
		if (hp + 5 < max_health)
			hp += 5;
		else
			hp = max_health;
		std::cout << "healed to " << hp << " out of " << max_health << std::endl;
		break;
	default:
		trainer_items.push_back(addition);
		break;
	}
	//limit the items to 10
	if (trainer_items.size() > 10)
	{
		std::cout << "cannot add more items than 10" << std::endl;
		trainer_items.pop_back();
		return;
	}
	//print the vec for the string
	for (Items i : trainer_items)
		std::cout << i.iid;
	std::cout << std::endl;
}

//currently locked between 1 and 999
int Trainer::increment_floor(int inc)
{
	if (floors + inc < 1)
	{
		throw("floors can't be less than one");
		return floors;
	}
	else if (floors + inc > 999)
	{
		throw("floors too high");
		return floors;
	}

	return floors += inc;
}

int Trainer::coin_helper(int val)
{
	if (trainer_coins + val > 9999)
	{
		trainer_coins = 9999;
		return trainer_coins;
	}
	return trainer_coins += val;
}
//return the ability the player is using
void(*Trainer::player_ability(int index))(Combatant *, Combatant *, int)
{
	void(*action)(Combatant*, Combatant*, int);

	//this is where limitations on which moves a trainer can do are imposed
	action = moves_returner(index);

	return action;
}
