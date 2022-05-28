#pragma once

#include <queue>

class Command
{
	
public:
	
	static enum class EType { MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, 
		SHOOT_UP, SHOOT_RIGHT, SHOOT_DOWN, SHOOT_LEFT};
	
	int id;
	std::queue<EType> type;

	//Command() {};
	//Command(EType _type) : type(_type) {};
};