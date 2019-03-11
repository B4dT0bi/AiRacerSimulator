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
	line[0].position = mWallhitA;
	line[0].color = sf::Color::Red;
	line[1].position = mWallhitB;
	line[1].color = sf::Color::Red;
	target.draw(line, 2, sf::Lines);
}

sf::Vector2f Line::extendLine(sf::Vector2f A, sf::Vector2f B, int len) const
{
	double lenAB = sqrt(pow(A.x - B.x, 2.0) + pow(A.y - B.y, 2.0));
	float x = B.x + (B.x - A.x) / lenAB * len;
	float y = B.y + (B.y - A.y) / lenAB * len;
	return sf::Vector2f(x, y);
}

void Line::update(sf::Vector2f A, sf::Vector2f B, const std::vector <collision::LineHitBox> roadHitBoxes, const sf::Transform& transform)
{
	mOriginalA = A;
	mOriginalB = B;
	mExtendedA = extendLine(A, B, 1000);
	mExtendedB = extendLine(A, B, -1000);
	mWallhitA= collision::minHitPoint(getHitBoxA(transform), roadHitBoxes);
	mDistanceToWallA = collision::calcDistance(getHitBoxA(transform).p1, mWallhitA);
	mWallhitB= collision::minHitPoint(getHitBoxB(transform), roadHitBoxes);
	mDistanceToWallB = collision::calcDistance(getHitBoxB(transform).p1, mWallhitB);
}

collision::LineHitBox Line::getHitBoxA(const sf::Transform &transform) const
{
	return collision::LineHitBox(transform.transformPoint(mOriginalA), transform.transformPoint(mExtendedA));
}

collision::LineHitBox Line::getHitBoxB(const sf::Transform &transform) const
{
	return collision::LineHitBox(transform.transformPoint(mOriginalB), transform.transformPoint(mExtendedB));
}

sf::Vector2f Line::getWallhitA() const
{
	return mWallhitA;
}

sf::Vector2f Line::getWallhitB() const
{
	return mWallhitB;
}

float Line::getDistanceToWallA() const
{
	return mDistanceToWallA;
}

float Line::getDistanceToWallB() const
{
	return mDistanceToWallB;
}
