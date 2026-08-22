#include "StdAfx.h"
#include "PlayerPaddle.h"
#include "Game.h"


PlayerPaddle::PlayerPaddle() :
_velocity(0),
_maxVelocity(600.0f)
{
	Load("images/paddle.png");
	assert(IsLoaded());

	GetSprite().SetCenter(GetSprite().GetSize().x /2, GetSprite().GetSize().y / 2);

}


PlayerPaddle::~PlayerPaddle()
{
}

void PlayerPaddle::Draw(sf::RenderWindow & rw)
{
	VisibleGameObject::Draw(rw);
}

float PlayerPaddle::GetVelocity() const
{
	return _velocity;
}

void PlayerPaddle::Update(float elapsedTime)
{

	if(Game::GetInput().IsKeyDown(sf::Key::Left))
	{
		_velocity-= 3.0f;
	}
	if(Game::GetInput().IsKeyDown(sf::Key::Right))
	{
		_velocity+= 3.0f;
	}

	if(Game::GetInput().IsKeyDown(sf::Key::Down))
	{
		_velocity= 0.0f;
	}

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