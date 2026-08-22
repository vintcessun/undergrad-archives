#include "StdAfx.h"
#include "AIPaddle.h"
#include "Game.h"
#include "GameBall.h"

AIPaddle::AIPaddle() :
_velocity(0),
_maxVelocity(600.0f)
{
	Load("images/paddle.png");
	assert(IsLoaded());

	GetSprite().SetCenter(GetSprite().GetSize().x /2, GetSprite().GetSize().y / 2);

}


AIPaddle::~AIPaddle()
{
}

void AIPaddle::Draw(sf::RenderWindow & rw)
{
	VisibleGameObject::Draw(rw);
}

float AIPaddle::GetVelocity() const
{
	return _velocity;
}

void AIPaddle::Update(float elapsedTime)
{
	const GameBall* gameBall = static_cast<GameBall*>(Game::GetGameObjectManager().Get("Ball"));
	sf::Vector2f ballPosition = gameBall->GetPosition();

	if(GetPosition().x -20 < ballPosition.x)
		_velocity += 15.0f;
	else if(GetPosition().x +20 > ballPosition.x)
		_velocity -= 10.0f;
	else
		_velocity = 0.0f;


	if(_velocity > _maxVelocity)
		_velocity = _maxVelocity;

	if(_velocity < -_maxVelocity)
		_velocity = -_maxVelocity;


	sf::Vector2f pos = this->GetPosition();

	if(pos.x  <= GetSprite().GetSize().x/2
		|| pos.x >= (Game::SCREEN_WIDTH - GetSprite().GetSize().x/2))
	{
		_velocity = -_velocity; // Bounce by current velocity in opposite direction
	}
	
	GetSprite().Move(_velocity * elapsedTime, 0);
}