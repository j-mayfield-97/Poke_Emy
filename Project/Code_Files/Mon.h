#pragma once
#include "../Combatant.h"
#include "Moves.h"//would the mon need to know if its caught and who its owner is?

//This was written early and needs to be restructed to my mist recent standards

class Mon: public Combatant
{
public: 
	Mon();
	Mon(int id);
	Mon(const char* _name, int att, int def, int _health) :
		Combatant(_name, att, def, _health) {
		set_ID();
	};
	Mon(const char* file_name);
	~Mon();

	bool catch_mon();
	
	void make_catchable();

	void hp_increase();

	void write_stats();
	void read_stats();
	void read_stats(int id);
	void printMoves();
	void printStats();

	//temp testing sprite path
	const char* ability_path1 = "assets/ufoo.png";
	const char* ability_path2 = "assets/ufoo.png";

	
protected:

	int type1; //tags for type ie reg = 0 , love = 1
	int type2;

	//to be implemented later
	//int magic;
	//int EX points; //points from battle and points to use magic
	//unique id
	//species id

	//this array is probably better than a vector
	//four moves max for now
	//blank moves array
	Moves moves[4] = {NULL,NULL,NULL,NULL};
	

	
};