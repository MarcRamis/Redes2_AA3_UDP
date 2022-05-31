#pragma once

#include <queue>

class CommandList
{
	
public:
	
	static enum class EType { MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, 
		SHOOT_UP, SHOOT_RIGHT, SHOOT_DOWN, SHOOT_LEFT};
	
	int id;
	std::queue<EType> type;
	float finalPosX, finalPosY;
	
	CommandList() {};
	CommandList(int _id, std::queue<EType> _type) : id(_id), type(_type) {};
	CommandList(int _id, std::queue<EType> _type, float _finalPosX, float _finalPosY) 
		: id(_id), type(_type), finalPosX(_finalPosX), finalPosY(_finalPosY) {};
};