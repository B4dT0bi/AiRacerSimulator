#include "mapEdit.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/View.hpp>

#include "RoadBlock.hpp"
#include "Map.hpp"
#include "Timer.hpp"

#include <iostream>

namespace mapEdit
{
Action::Action()
{
	placeUnplace = false;
	rotation = RoadBlock::standard;
	rtype = RoadBlock::straight;
	viewMove = sf::Vector2f(0, 0);
}

void getEvents(sf::RenderWindow &window, Action &action)
{
	//reset the actions
	action.placeUnplace = false;
	action.viewMove = sf::Vector2f(0, 0);

	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				window.close();
				break;
			case sf::Keyboard::Space:
				action.placeUnplace = true;
			default:
				break;
			}
			break;
		case sf::Event::KeyReleased:
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Right:
				action.rotation++;
				break;
			case sf::Mouse::Left:
				action.rtype++;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	//speeds in px per second
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		action.viewMove = sf::Vector2f(0, 256);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		action.viewMove = sf::Vector2f(0, -256);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		action.viewMove = sf::Vector2f(256, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		action.viewMove = sf::Vector2f(-256, 0);
	}
}

void mapEdit(sf::RenderWindow &window)
{
	sf::View mapView(sf::FloatRect(0, 0, 800, 600));
	window.setView(mapView);

	RoadBlock cursorBlock(RoadBlock::straight, RoadBlock::standard, sf::Vector2f(200, 200)); //block that will be drawn under the cusor, for the user to know what he is going to use

	Map editedMap; //map that will contain all that will be saved
	Action action;

	Timer loopTimer(sf::seconds(1. / 60.)); //60 fps

	//timer that is useful to calculate a time factor
	Timer timerFactorTimer; //no duration time is needed

	while (window.isOpen())
	{
		//RoadBlock block(action.rtype, action.rotation, sf::Vector2f(0, 0));

		mapEdit::getEvents(window, action);

		sf::Vector2i cursorBlockPosition = sf::Mouse::getPosition(window); //get the position, relative to the window
		cursorBlock.setPosition(window.mapPixelToCoords(cursorBlockPosition));
		cursorBlock.setRotation(action.rotation);
		cursorBlock.setType(action.rtype);

		if (action.placeUnplace)
		{
			//determine the size position of the block to be placedi
			sf::Vector2f posNewBlock((int(cursorBlock.getPosition().x) / 128) * 128, (int(cursorBlock.getPosition().y) / 128) * 128);
			if (cursorBlock.getPosition().x < 0)
			{
				posNewBlock.x -= 128;
			}
			if (cursorBlock.getPosition().y < 0)
			{
				posNewBlock.y -= 128;
			}

			editedMap.push_back(RoadBlock(action.rtype, action.rotation, posNewBlock));
		}

		//calculating the coordinates of the new view
		float viewFactor = timerFactorTimer.swapTime().asSeconds();

		//game display
		mapView.move(action.viewMove * viewFactor);
		window.setView(mapView);

		window.clear(sf::Color::Black);

		window.draw(editedMap);
		window.draw(cursorBlock);

		window.display();

		//time handling///////////////////////////

		loopTimer.autoSleep();

		// \time handling/////////////////////////
	}

	//finally : save to file !
	editedMap.saveToFile("saveMap.pwet");
}
} // namespace mapEdit
