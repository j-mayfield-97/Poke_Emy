#pragma once
#include<iostream>
class Moves
{
public:
	Moves();
	Moves(int arg, ...);
	~Moves();

	const char* name_move;
	int cost; //cost pp
	int limit;//times you can use it per battle
	int baseAtt;
	//theres should be an enum of effects
	int effectId;

	//quick function to evaluate if a move is unassigned or not
	bool operator == (int a)
	{
		if (a == NULL)
			return true;
		else
			return false;
	}

private:

};

