#pragma once
#include "../include/Paddle.h"

struct Position
{
	float x;
	float y;
};

struct Velocity
{
	float x;
	float y;
};

class Ball
{
public:
	Position position;
	Velocity velocity;
	bool collidesWithPaddle(Paddle paddle, int ballWidth = 15);
	void updateBallDirection();
	void reverseBallXDirection();
	void reverseBallYDirection();
	bool isBallOutOfBottomSide(float windowHeight = 600.0f, int ballWidth = 15);
	bool collidesWithTopWall(int ballWidth = 15);
	bool collidesWithLeftWall(int ballWidth = 15);
	bool collidesWithRightWall(float windowWidth = 1024.0f, int ballWidth = 15);

public:
};

