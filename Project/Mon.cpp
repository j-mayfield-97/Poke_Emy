#pragma once
//need this to use itoa without the deprecated warning
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE  

#include "Code_Files/Mon.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream> 
#include <string.h>

Mon::Mon()
{
	//init an empty mon, to later be filled in with a separate function
	name = "EMPTY";
}
Mon::Mon(int id)
{
	//init an empty mon, to later be filled in with a separate function
	name = "EMPTY";
}

Mon::Mon(const char* file_name) : Combatant(file_name)
{
	//needs moves in here
}

Mon::~Mon()
{

}


bool Mon::catch_mon()
{
	//make sure its alive
	alive_check();

	if (alive && hp < (max_health / 2))
	{
		//catch condition success
		std::cout << "caught " << name << std::endl;
		return true;
	}
	else
	{
		//failure
		return false;
	}
}


//just use to test if catching works
void Mon::make_catchable()
{
	hp = (hp / 2) - 1;
	std::cout << name << "has been made catchable" << std::endl;
}

void Mon::hp_increase()
{
	hp += 10;
	std::cout << name << " hp increases " << hp << "/" << max_health << std::endl;
}

void Mon::write_stats()
{
	//init json
	const char* json = "{\"Name\":\"nam\",\"Attack\":-1,\"Defense\":-1,\"HpTotal\":-1, \"Hp\":-1, \"sprite_path\":\"path\"}";

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

	rapidjson::Value& s_path = doc["sprite_path"];
	s_path.SetString(rapidjson::StringRef(inBattlePath));

	//concat the trainer name into the save file name
	std::string path = "SaveData/Mons/";
	std::string extension = ".json";

	char buffer[50];
	const char* iidd = itoa(get_ID(), buffer, 10);

	std::string truepath = path + "m" + iidd + extension;

	//open file, use the outstream wrapper to write to file
	std::ofstream ofs(truepath);
	rapidjson::OStreamWrapper osw(ofs);
	rapidjson::Writer<rapidjson::OStreamWrapper> writerwr(osw);
	doc.Accept(writerwr);

	//then close file
	ofs.close();

}

void Mon::read_stats()
{
	//concat the trainer name into the save file name
	std::string path = "SaveData/Mons/";
	std::string extension = ".json";

	std::string truepath = path + "m"  + extension;
	assert("dont use this, pass ID param instead");

	//read
	std::ifstream ifs(truepath);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	ifs.close();


	//access members then read them
	//and set the object to the values read
	rapidjson::Value& val_name = doc["Name"];
	//name = doc.Parse<0>().HasParseError();

	rapidjson::Value& val_att = doc["Attack"];
	attack = val_att.GetInt();

	rapidjson::Value& val_def = doc["Defense"];
	defense = val_def.GetInt();

	rapidjson::Value& val_hpt = doc["HpTotal"];
	max_health = val_hpt.GetInt();

	rapidjson::Value& val_hp = doc["Hp"];
	hp = val_hp.GetInt();

	rapidjson::Value& s_path = doc["sprite_path"];
	inBattlePath = s_path.GetString();
}

void Mon::read_stats(int id)
{
	//concat the trainer name into the save file name
	std::string path = "SaveData/Mons/";
	std::string extension = ".json";

	//convert uuid to a string
	char buffer[50];
 	const char* iidd = itoa(id, buffer, 10);

	std::string truepath = path + "m" +  iidd + extension;

	//read
	std::ifstream ifs(truepath);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	ifs.close();

	//set the ID member
	set_ID(id);

	//access members then read them
	//and set the object to the values read
	rapidjson::Value& val_name = doc["Name"];
	name = val_name.GetString();

	rapidjson::Value& val_att = doc["Attack"];
	attack = val_att.GetInt();

	rapidjson::Value& val_def = doc["Defense"];
	defense = val_def.GetInt();

	rapidjson::Value& val_hpt = doc["HpTotal"];
	max_health = val_hpt.GetInt();

	rapidjson::Value& val_hp = doc["Hp"];
	hp = val_hp.GetInt();

	rapidjson::Value& s_path = doc["sprite_path"];
	inBattlePath = s_path.GetString();
}

//these print funtions should push sdl text to the screen instead of printing to console
void Mon::printMoves()
{

	std::cout << name << " knows ";
	for (auto i : move_list)
	{
		std::cout << i.name_move << ", ";
	}
	std::cout << std::endl;
}

void Mon::printStats()
{
	std::cout << name << " Stats " << std::endl;

	std::cout << " def is " << defense << std::endl;
	std::cout << " hp is " << hp << std::endl;

	write_stats();
	alive_check();
	printMoves();
}

