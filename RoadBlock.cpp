#include "RoadBlock.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "FileLoader.hpp"
#include "collision.hpp"

#include <iostream>

const sf::Texture RoadBlock::straightTexture = createFromFile<sf::Texture>(STRAIGHT_TEXTURE_NAME);
const sf::Texture RoadBlock::goalTexture = createFromFile<sf::Texture>(GOAL_TEXTURE_NAME);
const sf::Texture RoadBlock::cornerTexture = createFromFile<sf::Texture>(CORNER_TEXTURE_NAME);
const sf::Texture RoadBlock::grassTexture = createFromFile<sf::Texture>(GRASS_TEXTURE_NAME);

const sf::Vector2i RoadBlock::texSize = sf::Vector2i(128, 128);

RoadBlock::RoadBlock()
{
	m_sprite.setOrigin(float(texSize.x) / 2.f, float(texSize.y) / 2.f);
}

RoadBlock::RoadBlock(const sf::Texture &texture, roadType t, rotation r, const sf::Vector2f &pos)
{
	setPosition(pos);

	m_sprite.setTexture(texture);
	setType(t);

	m_sprite.setOrigin(float(texSize.x) / 2.f, float(texSize.y) / 2.f);

	setRotation(r);

	m_sprite.setOrigin(0, 0);
}

RoadBlock::RoadBlock(roadType t, rotation r, const sf::Vector2f &pos)
{
	m_sprite.setOrigin(float(texSize.x) / 2.f, float(texSize.y) / 2.f);

	setPosition(pos);

	//std::cout<< pos.x<< " ; "<< pos.y<< '\n';

	setRotation(r);
	setType(t);
}

void RoadBlock::setRotation(rotation r)
{

	switch (r)
	{
	case right:
		m_sprite.setRotation(90);
		break;
	case left:
		m_sprite.setRotation(-90);
		break;
	case down:
		m_sprite.setRotation(180);
		break;
	case standard:
		m_sprite.setRotation(0);
		break;
	default:
		break;
	}
	//m_sprite.rotate(10);
}

void RoadBlock::setType(roadType t)
{
	m_type = t;

	switch (t)
	{
	case straight:
		m_sprite.setTexture(RoadBlock::straightTexture);
		break;
	case goal:
		m_sprite.setTexture(RoadBlock::goalTexture);
		break;
	case corner:
		m_sprite.setTexture(RoadBlock::cornerTexture);
		break;
	default:
		break;
	}
}

RoadBlock::rotation RoadBlock::getRotation() const
{
	float r = m_sprite.getRotation();
	//std::cout<< r<< '\n';
	switch (int(r))
	{
	case 90:
		return right;
		break;
	case 270:
		return left;
		break;
	case 180:
		return down;
		break;
	default:
		break;
	}
	return standard; //shall never be executed
}

float RoadBlock::getRotationAsDegrees() const
{
	return m_sprite.getRotation();
}

RoadBlock::roadType RoadBlock::getRType() const
{
	return m_type;
}

RoadBox RoadBlock::getHitBox() const
{
	return RoadBox(*(this));
}

void RoadBlock::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.transform.translate(float(texSize.x) / 2.f, float(texSize.y) / 2.f);

	target.draw(m_sprite, states);
}

RoadBlock::rotation operator++(RoadBlock::rotation &r, int nn)
{
	RoadBlock::rotation ans(r); //answer
	switch (r)
	{
	case RoadBlock::standard:
		r = RoadBlock::right;
		break;
	case RoadBlock::right:
		r = RoadBlock::down;
		break;
	case RoadBlock::down:
		r = RoadBlock::left;
		break;
	case RoadBlock::left:
		r = RoadBlock::standard;
		break;
	default:
		break;
	}
	return ans;
}

RoadBlock::rotation operator--(RoadBlock::rotation &r, int nn)
{
	RoadBlock::rotation ans(r); //answer
	switch (r)
	{
	case RoadBlock::standard:
		r = RoadBlock::left;
		break;
	case RoadBlock::right:
		r = RoadBlock::standard;
		break;
	case RoadBlock::down:
		r = RoadBlock::right;
		break;
	case RoadBlock::left:
		r = RoadBlock::down;
		break;
	default:
		break;
	}
	return ans;
}

RoadBlock::roadType operator++(RoadBlock::roadType &r, int)
{
	RoadBlock::roadType ans(r);
	switch (r)
	{
	case RoadBlock::straight:
		r = RoadBlock::corner;
		break;
	case RoadBlock::corner:
		r = RoadBlock::goal;
		break;
	case RoadBlock::goal:
		r = RoadBlock::straight;
		break;
	default:
		break;
	}
	return ans;
}
