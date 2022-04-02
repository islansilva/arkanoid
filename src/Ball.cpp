#include "../include/Ball.h"

const float UPDATE_VELOCITY_ON_COLISION = 1.0f;

bool Ball::collidesWithPaddle(Paddle paddle, int ballWidth)
{
	float paddleFinalHeight = paddle.getY() + paddle.getHeight();
	float ballFinalHeight = this->position.y + ballWidth;
	float paddleFinalWidth = paddle.getX() + paddle.getWidth();
	float ballFinalWidth = this->position.x + ballWidth;

	return (this->position.x >= paddle.getX()
		&& this->position.x <= paddleFinalWidth
		&& ballFinalHeight <= paddleFinalHeight
		&& ballFinalHeight >= paddle.getY());
}

void Ball::updateBallDirection()
{
	if (this->velocity.y > 0)
		this->velocity.y += UPDATE_VELOCITY_ON_COLISION;
	else
		this->velocity.y -= UPDATE_VELOCITY_ON_COLISION;

	this->velocity.y *= -1.0f;
}

void Ball::reverseBallXDirection()
{
	this->velocity.x *= -1.0f;
}

void Ball::reverseBallYDirection()
{
	this->velocity.y *= -1.0f;
}

bool Ball::isBallOutOfBottomSide(float windowWidth, int ballWidth)
{
	return (this->position.y > windowWidth);
}

bool Ball::collidesWithTopWall(int ballWidth)
{
	if (this->position.y <= ballWidth && this->velocity.y < 0.0f)
		return true;

	return false;
}

bool Ball::collidesWithLeftWall(int ballWidth)
{
	return (this->position.x <= 0);
}

bool Ball::collidesWithRightWall(float windowWidth, int ballWidth)
{
	return (this->position.x >= windowWidth/2 - ballWidth);
}
