#pragma once

#include "Code_Files/Mon.h"

Mon::Mon()
{
	//init an empty mon , to later be filled in with a separate function
	name = "EMPTY";
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
}
Mon::~Mon()
{

}