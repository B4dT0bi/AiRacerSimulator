#include "RoadBlock.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


RoadBlock::RoadBlock()
{
}


RoadBlock::RoadBlock(const sf::Texture &texture, roadType t, rotation r, const sf::Vector2f &pos)
{
	setPosition(pos);

	m_sprite.setTexture(texture);
	setType(t);
	
	sf::Vector2u texSize(texture.getSize());
	m_sprite.setOrigin(float(texSize.x)/2.f, float(texSize.y)/2.f);
	
	setRotation(r);
}



RoadBlock::RoadBlock(roadType t, rotation r, const sf::Vector2f &pos)
{
	setPosition(pos);

	setRotation(r);
	setType(t);
}




void RoadBlock::setRotation(rotation r)
{
	switch(r)
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
		default:
			break;
	}
}




void RoadBlock::setType(roadType t)
{
	m_type = t;

	switch(t)
	{
		case straight:
			m_sprite.setTexture(straightTexture);
			break;
		case corner:
			m_sprite.setTexture(cornerTexture);
			break;
		default:
			break;
	}
}


void RoadBlock::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states *= getTransform();

	target.draw(m_sprite, states);
}
