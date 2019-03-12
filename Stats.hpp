/**
* Show stats.
**/
#ifndef DEF_STATS_HPP
#define DEF_STATS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include "Car.hpp"

class Stats : public sf::Drawable, public sf::Transformable
{
  public:
	Stats(Car &car);
	void update();
	void lapCompleted();

  protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  private:
	sf::Text m_speedIndicator;
	sf::Text m_txt_score;
	sf::Text m_txt_laps;
	sf::Text m_txt_lap_time;
	sf::Text m_txt_lap_time_best;
	sf::Text m_txt_total_time;
	sf::Font m_font;
	sf::Clock clockLap;
	sf::Clock clockTotal;
	
	sf::Time m_lap_time;
	sf::Time m_lap_time_best;
	sf::Time m_total_time;
	int m_laps;
	int m_score;
	Car &m_car;
	void initTextField(sf::Text &field, const sf::Vector2f &pos, const sf::String initValue);
	sf::String convert(sf::Time &time);
};

#endif
