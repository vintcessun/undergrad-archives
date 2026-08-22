#include "StdAfx.h"
#include "SplashScreen.h"


void SplashScreen::Show(sf::RenderWindow & renderWindow)
{
	sf::Image image;
	if(image.LoadFromFile("images/SplashScreen.png") != true)
	{
		return;
	}

	sf::Sprite sprite(image);
	
	renderWindow.Draw(sprite);
	renderWindow.Display();

	sf::Event event;
	while(true)
	{
		renderWindow.GetEvent(event);
		if(event.Type == sf::Event::KeyPressed 
			|| event.Type == sf::Event::MouseButtonPressed
			|| event.Type == sf::Event::Closed )
		{
			return;
		}
	}
}