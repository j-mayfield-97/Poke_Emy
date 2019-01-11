#pragma once
#include<iostream>
class Moves
{
public:
	Moves(const char* title, int _move_id, int _cost, int _limit, int _att);
	Moves(int arg);
	~Moves();


	//ACTUALLY there should be functions that return the moves values from the json file based on the move_id
	//!!!!!!!!!

	const char* name_move;
	int move_id;
	int cost_use; //cost pp
	int limit_use;//times you can use it per battle
	int baseAtt;
	//theres should be an enum of effects
	int effectId;

	//quick function to evaluate if a move is unassigned or not
	bool operator == (int a)
	{
		if (a == NULL)
			return false;
		else
			return true;
	}

	bool operator == (Moves mov)
	{
		if (mov.move_id == this->move_id)
			return true;
		else
			return false;
	}

	bool isNULL()
	{
		if (this == nullptr)
			return true;
		else if (this->move_id < 0 || this->move_id > 100)
			return true;
		else
			return false;
	}

private:

};

