#pragma once
#include <vector>

struct PositionBlock
{
	float x;
	float y;
};

class Block
{
public:
	PositionBlock position;
	float heigth;
	float width;
	int color;

	bool Initialize(float x, float y) { return true; };
	void Destruct();

private:
	
	
};