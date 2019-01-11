#pragma once
#include "../Combatant.h"
#include "Moves.h"//would the mon need to know if its caught and who its owner is?

//This was written early and needs to be restructed to my mist recent standards

class Mon: public Combatant
{
public: 
	Mon();
	Mon(const char* _name, int att, int def, int _health) :
		Combatant(_name, att, def, _health) {};
	~Mon();

	bool catch_mon();
	
	void make_catchable();

	void hp_increase();


	void printMoves();
	void printStats();
protected:

	int type1; //tags for type ie reg = 0 , love = 1
	int type2;

	//to be implemented later
//	int magic;
//	int EX points; //points from battle and points to use magic
	//unique id
	//species id

	//this array is probably better than a vector
	//four moves max for now
	//blank moves array
	Moves moves[4] = {NULL,NULL,NULL,NULL};
	

	
};