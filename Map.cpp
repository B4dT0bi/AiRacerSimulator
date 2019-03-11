#include "Map.hpp"
#include <string>
#include <SFML/Graphics/RenderTarget.hpp>
#include "RoadBlock.hpp"

#include <iostream>
#include <fstream>

bool m_show_road_hitbox = false;

Map::Map()
{
}

Map::Map(const std::string &fileName)
{
	loadFromFile(fileName);
}

void Map::loadFromFile(const std::string &fileName)
{
	std::ifstream fichier(fileName.c_str(), std::ios::in);

	if (fichier)
	{
		while (true)
		{
			int rotation;
			int rtype;
			float x, y;

			fichier >> rotation >> rtype >> x >> y;

			if (fichier.eof())
			{
				break; //We stop the loop if we reach the end of the file
			}

			RoadBlock readFile(static_cast<RoadBlock::roadType>(rtype), static_cast<RoadBlock::rotation>(rotation), sf::Vector2f(x, y));
			m_BlockList.push_back(readFile);
		}
	}
	else
		std::cerr << "Error 404 :: Cannot open file !" << std::endl;
}

void Map::saveToFile(const std::string &fileName)
{
	std::ofstream fichier(fileName.c_str(), std::ios::out);

	if (fichier)
	{
		for (std::list<RoadBlock>::iterator it = m_BlockList.begin(); it != m_BlockList.end(); it++)
		{
			float x, y;

			sf::Vector2f pos(it->getPosition());

			x = pos.x;
			y = pos.y;

			std::cout << x << " ; " << y << '\n';

			fichier << static_cast<RoadBlock::rotation>(it->getRotation()) << ' ' << static_cast<RoadBlock::roadType>(it->getRType()) << ' ' << x << ' ' << y << '\n';
		}
	}
	else
		std::cerr << "Error 404 :: Cannot save to file !" << std::endl;
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (std::list<RoadBlock>::const_iterator it = m_BlockList.begin(); it != m_BlockList.end(); it++)
	{
		target.draw(*it, states);
		if (m_show_road_hitbox)
		{
			// draw hitbox
			const std::vector<collision::LineHitBox> &hitBox = it->getHitBox().getLineArray();

			for (unsigned int i = 0; i < hitBox.size(); i++)
			{
				if (hitBox[i].p1.x > 0 && hitBox[i].p1.y > 0)
				{
					sf::Vertex line[2];
					line[0].position = hitBox[i].p1;
					line[0].color = sf::Color::Green;
					line[1].position = hitBox[i].p2;
					line[1].color = sf::Color::Green;
					target.draw(line, 2, sf::Lines);
				}
			}
		}
	}
}

void Map::push_back(const RoadBlock &RdBk)
{
	bool found = false;
	sf::Vector2f pos(RdBk.getPosition());

	for (std::list<RoadBlock>::iterator it = m_BlockList.begin(); it != m_BlockList.end() && !found; it++)
	{
		found = it->getPosition() == pos; //Replace an eventual element that is at the same place in the map
		if (found)
		{
			*it = RdBk;
		}
	}

	if (!found)
	{
		m_BlockList.push_back(RdBk);
	}
}

Map::iterator Map::begin()
{
	return m_BlockList.begin();
}

Map::iterator Map::end()
{
	return m_BlockList.end();
}

void Map::toggleShowHitbox()
{
	m_show_road_hitbox = !m_show_road_hitbox;
}


/*
namespace hidden
{
	MapIterator::MapIterator()
	{
	}


	MapIterator operator++(int n)
	{
		MapIterator ans(*this);
		if(!m_listIterator.
	}
}
*/
