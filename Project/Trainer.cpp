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

//trainer and mon should probably inherent from common a "combatant" class


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

		delete mn;
	}
	//LOAD  floors from json!
	floors = 0;
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

// A random pokemon chosen then add that pokemon 
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
		return;
	//vector rotation 
	//theres a function for this in std but it seems unecessary to bring it in
	Mon* hold;
	hold = mons.back();
	mons.pop_back();
	mons.insert(mons.begin(), hold);
}
