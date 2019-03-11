#include "game.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>
#include "unistd.h"

#include "Map.hpp"
#include "Timer.hpp"
#include "OpenFileError.hpp"
#include "Stats.hpp"

namespace game
{
Action::Action(float orien, int accel) : orientation(orien), acceleration(accel)
{
}

void getEvents(sf::RenderWindow &window, Action &action, Car &car, Map &map)
{
	action.acceleration = 0;
	action.orientation = 0;
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			exit(EXIT_SUCCESS);
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				exit(EXIT_SUCCESS);
				break;
			case sf::Keyboard::H:
				map.toggleShowHitbox();
				break;
			case sf::Keyboard::L:
				car.toggleShowDistanceLines();
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyReleased:
			break;
		default:
			break;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		action.acceleration = -100;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		action.acceleration = 100;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		action.orientation = 5;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		action.orientation = -5;
	}
}

void game(sf::RenderWindow &window)
{
	//view that will follow the car
	sf::View carView(sf::FloatRect(0, 0, 800, 600));
	//carView.setSize(640, 480);
	window.setView(carView);

	//image loading
	sf::Texture texPlayerCar;
	if (!texPlayerCar.loadFromFile(CAR_FILE))
	{
		throw OpenFileError();
	}

	Map map(std::string("saveMap.pwet"));

	//std::cout << "map loaded " << map.begin()->getPosition().x << " ; " << map.begin()->getPosition().y << '\n';

	Car playerCar(texPlayerCar, 200);
	Stats stats(playerCar);
	playerCar.setPosition(sf::Vector2f(RoadBlock::texSize / 2));
	//std::cout << "playerCar " << playerCar.getPosition().x << " ; " << playerCar.getPosition().y << '\n';
	//sound loading

	//music loading

	//other variables
	Action action;

	Timer loopTimer(sf::seconds(1. / 60.)); //60 fps
	//std::cout<< loopTimer.getDuration().asSeconds()<< '\n';
	loopTimer.restart();

	//main loop
	while (true)
	{
		getEvents(window, action, playerCar, map);

		//game physic/////////////////////////////
		playerCar.accelerate(action.acceleration);
		playerCar.rotate(action.orientation);

		playerCar.apply_physics(map);
		stats.update();

		//collisions tests
		/*bool collided = false;
			int i = 0;
			for(Map::iterator it = map.begin(); it != map.end() && !collided; it++)
			{	
				collided = collision::collision(playerCar.getHitBox(), it->getHitBox());
			}
			if(collided)
			{
				std::cout<< ++j<<"\n";
			}*/

		// \game physics /////////////////////////

		//game display////////////////////////////
		//carView.setCenter(playerCar.getPosition());
		window.setView(carView);

		window.clear(sf::Color::White);

		window.draw(map);
		window.draw(playerCar);
		window.draw(stats);

		window.display();

		// \game display//////////////////////////

		//time handling///////////////////////////

		loopTimer.autoSleep();
		//while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		//	usleep(100);
		//}

		// \time handling/////////////////////////
	}
}

void loadCars(std::vector<Car> &carsTab, std::vector<sf::Texture> &texTab)
{
	try
	{
		sf::Texture texCar;
		if (!texCar.loadFromFile(CAR_FILE))
		{
			OpenFileError error;
			throw error;
		}

		texTab.push_back(texCar);
		carsTab.push_back(Car(texTab[0], CAR_SPEED));
	}
	catch (std::exception &except)
	{
		std::cerr << except.what() << "\n";
	}
}

} // namespace game

void keepCarOnRoad(Car &car, Map &map, Map::iterator &it)
{
}
