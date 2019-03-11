#include "Stats.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>
#include <sstream> //type conversion
#include <iostream>
#include <cassert>

Stats::Stats(Car &car) : m_car(car)
{
	m_font.loadFromFile("gameData/fonts/CANON.ttf");
	m_speedIndicator.setFont(m_font);
	m_speedIndicator.setFillColor(sf::Color::Black);
	m_speedIndicator.setPosition(sf::Vector2f(700., 0.));

	m_speedIndicator.setString(std::to_string((int)m_car.getSpeed()));
	m_speedIndicator.setCharacterSize(32);

	m_score.setFont(m_font);
	m_score.setFillColor(sf::Color::Black);
	m_score.setPosition(sf::Vector2f(700., 50.));

	m_score.setString("0");
	m_score.setCharacterSize(32);
}

void Stats::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_speedIndicator);
	target.draw(m_score);
}

void Stats::update()
{
	m_speedIndicator.setString(std::to_string((int)m_car.getSpeed()));
}
