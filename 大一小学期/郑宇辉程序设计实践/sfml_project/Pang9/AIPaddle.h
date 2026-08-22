#pragma once
#include "visiblegameobject.h"
class AIPaddle :
	public VisibleGameObject
{
public:
	AIPaddle(void);
	~AIPaddle(void);

	void Update(float elapsedTime);
	void Draw(sf::RenderWindow& rw);
	
	float GetVelocity() const;

private:
	float _velocity;  // -- left ++ right
	float _maxVelocity;
};

