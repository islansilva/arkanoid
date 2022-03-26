#include "../include/Block.h"

bool Block::Initialize(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
	this->heigth = 10;
	this->width = 30;

	return true;
}


