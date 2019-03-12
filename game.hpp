#ifndef DEF_GAME_HPP
#define DEF_GAME_HPP

#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <string>

#include "Car.hpp"
#include "Map.hpp"

const unsigned int CAR_SPEED = 10;
const unsigned int CAR_ACCELERATION = 1;
const std::string CAR_FILE("gameData/images/carOrange.png");

//const unsigned int KEY_EVENT_NUMBER = sf::Keyboard::KeyCount;

namespace sf
{
class RenderWindow;
}

class Car;

namespace game
{
struct Action
{
	Action(float orien = 0, int accel = 0);

	float orientation;
	int acceleration;
};

void loadCars(std::vector<Car> &carsTab, std::vector<sf::Texture> &texTab);

void game(sf::RenderWindow &window, std::string &mapFile);

void getEvents(sf::RenderWindow &window, Action &action);

void keepCarOnRoad(Car &car, Map &map, Map::iterator &it);
} // namespace game

#endif
