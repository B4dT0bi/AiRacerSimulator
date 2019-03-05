#include "Line.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <cmath>
#include <sstream> //type conversion
#include <iostream>
#include <cassert>

Line::Line()
{
}

void Line::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//states.transform *= getTransform();
	sf::Vertex line[2];
	line[0].position = mExtendedA;
	line[0].color = sf::Color::Red;
	line[1].position = mExtendedB;
	line[1].color = sf::Color::Red;
	target.draw(line, 2, sf::Lines, states);
}

sf::Vector2f Line::extendLine(sf::Vector2f A, sf::Vector2f B, int len) const
{
	double lenAB = sqrt(pow(A.x - B.x, 2.0) + pow(A.y - B.y, 2.0));
	float x = B.x + (B.x - A.x) / lenAB * len;
	float y = B.y + (B.y - A.y) / lenAB * len;
	return sf::Vector2f(x, y);
}

void Line::update(sf::Vector2f A, sf::Vector2f B)
{
	mOriginalA = A;
	mOriginalB = B;
	mExtendedA = extendLine(A, B, 1000);
	mExtendedB = extendLine(A, B, -1000);
}

collision::LineHitBox Line::getHitBoxA(const sf::Vector2f pos) const
{
	return collision::LineHitBox (mOriginalA + pos, mExtendedA + pos);

}

collision::LineHitBox Line::getHitBoxB(const sf::Vector2f pos) const
{
	return collision::LineHitBox(mOriginalB + pos, mExtendedB + pos);
}
