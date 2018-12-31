#pragma once

#include "Code_Files/Mon.h"

Mon::Mon()
{
	//init an empty mon, to later be filled in with a separate function
	name = "EMPTY_Mon";
	alive = true;
	type1 = NULL;
	type2 = NULL;
	outBattlePath = "EMPTY.PNG";
	inBattlePath = "EMPTY.PNG";
	//init the stats of a blank mon
	attack = -1;
	defense = -1;
	originalHp = -1;
	hp = -1;
	pp = -1;
	
	//init moves and learn some
	Moves first = Moves("first");
	Moves sec = Moves("sec");
	Moves thr = Moves("thr");
	Moves frth = Moves("frth");
	learnMoves(first);
	learnMoves(sec);
	learnMoves(thr);
	learnMoves(frth);

}
Mon::~Mon()
{

}