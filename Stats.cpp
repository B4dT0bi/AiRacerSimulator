#include "Stats.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

#include <cmath>
#include <sstream> //type conversion
#include <iostream>
#include <cassert>

float distance = 20.0;
int font_size = 20;

Stats::Stats(Car &car) : m_car(car)
{
	m_font.loadFromFile("gameData/fonts/Xenogears.ttf");

	initTextField(m_speedIndicator, sf::Vector2f(700., 0.), std::to_string((int)m_car.getSpeed()));
	initTextField(m_txt_score, sf::Vector2f(700., distance), "Score : 0");
	initTextField(m_txt_laps, sf::Vector2f(700., distance * 2), "Laps : 0");
	initTextField(m_txt_lap_time, sf::Vector2f(700., distance * 3), "Lap time : 0");
	initTextField(m_txt_lap_time_best, sf::Vector2f(700., distance * 4), "Best lap : 0");
	clockTotal.restart();
	clockLap.restart();
}

sf::String Stats::convert(sf::Time &time) 
{
	sf::Int32 ms = time.asMilliseconds() % 1000;
	sf::Int32 sec = (time.asMilliseconds() / 1000) % 60;
	sf::Int32 min = (time.asMilliseconds() /1000 / 60) % 60;
	sf::Int32 hour = (time.asMilliseconds() /1000 / 60 / 60) % 60;;
	sf::String result("");
	if (hour > 0) {
		result = std::to_string((int) hour) + ":";
	}
	if (min > 0) {
		result = result + std::to_string((int) min) + ":";
	}
	result = result + std::to_string((int) sec) + "." + std::to_string((int) ms);
	return result;
}

void Stats::lapCompleted() 
{
	m_laps++;
	sf::Time elapsed = clockLap.restart();

}

void Stats::initTextField(sf::Text &field, const sf::Vector2f &pos, const sf::String initValue) 
{
	field.setFont(m_font);
	field.setFillColor(sf::Color::Black);
	field.setPosition(pos);
	field.setString(initValue);
	field.setCharacterSize(font_size);
}

void Stats::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_speedIndicator);
	target.draw(m_txt_score);
	target.draw(m_txt_laps);
	target.draw(m_txt_lap_time);
	target.draw(m_txt_lap_time_best);
}

void Stats::update()
{
	m_speedIndicator.setString(std::to_string((int)m_car.getSpeed()));
	m_txt_score.setString(std::to_string((int)m_score));
	m_txt_laps.setString(std::to_string((int)m_laps));
	m_lap_time = clockLap.getElapsedTime();
	m_txt_lap_time.setString(convert(m_lap_time));
}
