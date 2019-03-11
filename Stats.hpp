/**
* Show stats.
**/
#ifndef DEF_STATS_HPP
#define DEF_STATS_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Car.hpp"

class Stats : public sf::Drawable, public sf::Transformable
{
  public:
	Stats(Car &car);
	void update();

  protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  private:
	sf::Text m_speedIndicator;
	sf::Text m_score;
	sf::Font m_font;
	Car &m_car;
};

#endif
