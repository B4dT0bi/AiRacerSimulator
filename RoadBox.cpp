#include "RoadBox.hpp"
#include "RoadBlock.hpp"

#include <iostream>

/*
All functions defined as static in this file are initializers for RoadBox's static variables (hit boxes).
Therefore there is no mean to give access to these in other files.
*/

namespace pv //private
{
static std::vector<collision::LineHitBox> getBigArcHitBox();
static std::vector<collision::LineHitBox> getSmallArcHitBox();
static std::vector<collision::LineHitBox> getStraightHitBox();
} // namespace pv

namespace pv //private
{
static std::vector<collision::LineHitBox> getBigArcHitBox()
{
	using namespace collision;

	sf::Vector2f posOffsetToCenter(-RoadBlock::texSize / 2);

	std::vector<LineHitBox> hitBox(3);
	hitBox[0] = LineHitBox(sf::Vector2f(31 / 2, 256 / 2), sf::Vector2f(31 / 2, 134 / 2));
	hitBox[1] = LineHitBox(sf::Vector2f(31 / 2, 134 / 2), sf::Vector2f(121 / 2, 32 / 2));
	hitBox[2] = LineHitBox(sf::Vector2f(121 / 2, 32 / 2), sf::Vector2f(256 / 2, 31 / 2));

	//center the hitBox on (0, 0)
	/*for(unsigned int i = 0; i < hitBox.size(); i++)
		{
			hitBox[i].move(posOffsetToCenter);
		}*/

	return hitBox;
}

static std::vector<collision::LineHitBox> getSmallArcHitBox()
{
	using namespace collision;

	sf::Vector2f posOffsetToCenter(-RoadBlock::texSize / 2);

	std::vector<LineHitBox> hitBox(2);
	hitBox[0] = LineHitBox(sf::Vector2f(225 / 2, 255 / 2), sf::Vector2f(233 / 2, 236 / 2));
	hitBox[1] = LineHitBox(sf::Vector2f(233 / 2, 236 / 2), sf::Vector2f(256 / 2, 225 / 2));

	//center the hitBox on (0, 0)
	/*for(unsigned int i = 0; i < hitBox.size(); i++)
		{
			hitBox[i].move(posOffsetToCenter);
		}*/

	return hitBox;
}

static std::vector<collision::LineHitBox> getStraightHitBox()
{
	using namespace collision;

	sf::Vector2f posOffsetToCenter(-RoadBlock::texSize / 2);

	std::vector<LineHitBox> hitBox(2);
	hitBox[0] = LineHitBox(sf::Vector2f(31 / 2, 256 / 2), sf::Vector2f(31 / 2, 0) /*sf::Vector2f(0, 0), sf::Vector2f(0, 256)*/);
	hitBox[1] = LineHitBox(sf::Vector2f(225 / 2, 256 / 2), sf::Vector2f(225 / 2, 0) /*sf::Vector2f(256, 0), sf::Vector2f(256, 256)*/);

	//center the hitBox on (0, 0)
	/*for(unsigned int i = 0; i < hitBox.size(); i++)
		{
			hitBox[i].move(posOffsetToCenter);
		}*/

	return hitBox;
}

} //namespace pv

const std::vector<collision::LineHitBox> RoadBox::bigArcHitBox = pv::getBigArcHitBox();
const std::vector<collision::LineHitBox> RoadBox::smallArcHitBox = pv::getSmallArcHitBox();
const std::vector<collision::LineHitBox> RoadBox::straightHitBox = pv::getStraightHitBox();

RoadBox::RoadBox(const RoadBlock &roadBlock)
{
	using namespace collision;

	//to get the right hit box, we need to transform the default hit box, because no rotation is in it
	sf::Transform transf;
	transf.rotate(roadBlock.getRotationAsDegrees(), sf::Vector2f(RoadBlock::texSize / 2));
	std::cout<< (RoadBlock::texSize/2).x<< " ; "<< (RoadBlock::texSize/2).y<< '\n';
	std::cout<< roadBlock.sf::Transformable::getRotation()<< '\n';

	RoadBlock::roadType t = roadBlock.getRType();
	switch (t)
	{
	case RoadBlock::straight:
	{
		m_hitBox.resize(straightHitBox.size());
		for (unsigned int i = 0; i < m_hitBox.size(); i++)
		{
			m_hitBox[i] = LineHitBox(
				transf.transformPoint(straightHitBox[i].p1), transf.transformPoint(straightHitBox[i].p2));
		}
	}
	break;
	case RoadBlock::corner:
	{
		m_hitBox.resize(bigArcHitBox.size() + smallArcHitBox.size());
		unsigned int i;
		for (i = 0; i < bigArcHitBox.size(); i++)
		{
			m_hitBox[i] = LineHitBox(
				transf.transformPoint(bigArcHitBox[i].p1), transf.transformPoint(bigArcHitBox[i].p2));
		}
		//std::cout<< bigArcHitBox.size();
		for (i = 0; i < smallArcHitBox.size(); i++)
		{
			m_hitBox[i + bigArcHitBox.size()] = LineHitBox(
				transf.transformPoint(smallArcHitBox[i].p1), transf.transformPoint(smallArcHitBox[i].p2));
		}
	}
	break;
	default: //shall never be used
		break;
	}

	//place the calculated hit boxes at the right position
	sf::Vector2f pos(roadBlock.getPosition());
	for (unsigned int i = 0; i < m_hitBox.size(); i++)
	{
		m_hitBox[i].p1 += pos;
		m_hitBox[i].p2 += pos;
	}
}

const std::vector<collision::LineHitBox> &RoadBox::getLineArray() const
{
	return m_hitBox;
}
