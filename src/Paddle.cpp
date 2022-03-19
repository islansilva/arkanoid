#include "../include/Paddle.h"

const float windowHalf = 512.0f;//tamanho da tela


bool Paddle::Initialize(float x, float y, float width, float height, int direction)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->direction = direction;

	return true;
}

void Paddle::changePaddleDirection(int direction)
{
	this->direction += direction;
}

bool Paddle::isPaddleMoving(int newDirection)
{
	if (newDirection != 0)
		return true;
	return false;
}

void Paddle::updatePaddlePosition(int newDirection, float deltaTime, int wallWidth)
{
	this->x += newDirection * windowHalf * deltaTime;

	if (this->x < 0)
	{
		this->x = 0;
	}
	else if (this->x > windowHalf - this->width)
	{
		this->x = windowHalf - this->width;
	}
}

float Paddle::getX()
{
	return this->x;
}

float Paddle::getY()
{
	return this->y;
}

float Paddle::getWidth()
{
	return this->width;
}

float Paddle::getHeight()
{
	return this->height;
}
