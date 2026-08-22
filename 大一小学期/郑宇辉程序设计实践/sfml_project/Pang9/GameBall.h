#pragma once
#include "visiblegameobject.h"


class GameBall :
	public VisibleGameObject
{
public:
	GameBall();
	virtual ~GameBall();
	void Update(float);

private:
	float _velocity;
	float _angle;
	float _elapsedTimeSinceStart;
	bool _runYet;

	float LinearVelocityX(float angle);
	float LinearVelocityY(float angle);
};

