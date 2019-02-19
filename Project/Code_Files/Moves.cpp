#pragma once

#include "Moves.h"

Moves::Moves(const char * title, int _move_id, int _cost, int _limit, int att)
{
	name_move = title;
	move_id = _move_id;
	cost_use = _cost;
	limit_use = _limit;
	baseAtt = att;

}

Moves::Moves(int arg)
{
	//wont be usefull after json parser implimented
}
Moves::~Moves()
{

}