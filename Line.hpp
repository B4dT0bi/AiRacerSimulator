/**
* A Car shall be drawable and transformable. It can collide with other objects.
**/
#ifndef DEF_LINE_HPP
#define DEF_LINE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "collision.hpp"

class Line : public sf::Drawable, public sf::Transformable
{

  public:
	Line();

	void update(sf::Vector2f A, sf::Vector2f B);
	collision::LineHitBox getHitBoxA(const sf::Vector2f pos) const;
	collision::LineHitBox getHitBoxB(const sf::Vector2f pos) const;

  protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  private:
	sf::Vector2f extendLine(sf::Vector2f A, sf::Vector2f B, int len) const;
	sf::Vector2f mOriginalA;
	sf::Vector2f mOriginalB;
	sf::Vector2f mExtendedA;
	sf::Vector2f mExtendedB;
};

#endif
