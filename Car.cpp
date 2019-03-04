#include "Car.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <cmath>
#include <sstream> //type conversion
#include <iostream>
#include <cassert>

#include "Map.hpp"
#include "collision.hpp"

Line m_line_back_to_front;
Line m_line_back_left_to_right;
Line m_line_front_left_to_right;
Line m_line_diagonal1;
Line m_line_diagonal2;

Car::Car(sf::Texture &tex, float maxSpeed)
{
	m_acceleration = 0;
	m_rotation = 0;

	m_sprite.setTexture(tex);

	sf::Vector2u texSize(tex.getSize());
	m_sprite.setOrigin(float(texSize.x) / 2.f, float(texSize.y) / 2.f);

	m_hitBoxRadius = 93 / 8;

	m_maxSpeed = maxSpeed;
	m_speed = 0;

	m_physicTimer.setDuration(sf::seconds(1. / 60.)); //60 fps
	m_physicTimer.restart();
}

void Car::accelerate(float accel)
{
	m_acceleration = accel;
}

void Car::rotate(float rot)
{
	m_rotation = rot;
}
Map *m_map;

void Car::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);

	// front / back
	target.draw(m_line_back_to_front, states);
	// front sides
	target.draw(m_line_front_left_to_right, states);
	// back sides
	target.draw(m_line_back_left_to_right, states);

	// diagonal
	target.draw(m_line_diagonal1, states);
	target.draw(m_line_diagonal2, states);

	// draw circle hitbox
	for (Map::iterator it = m_map->begin(); it != m_map->end(); it++)
	{

		const std::vector<collision::LineHitBox> &hitBox = it->getHitBox().getLineArray();

		for (unsigned int i = 0; i < hitBox.size(); i++)
		{
			if (hitBox[i].p1.x > 0 && hitBox[i].p1.y > 0)
			{
				std::cout << "hitbox[" << i << "] p1 x=" << hitBox[i].p1.x << " y=" << hitBox[i].p1.y << "\n";
				std::cout << "hitbox[" << i << "] p2 x=" << hitBox[i].p2.x << " y=" << hitBox[i].p2.y << "\n";
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

void Car::apply_physics(Map &map)
{
	m_map = &map;
	if (m_physicTimer.ticked())
	{
		//float currentSpeed = norm(m_speedVector);

		sf::Transformable::rotate(m_rotation /**(currentSpeed / m_maxSpeed)*/);
		float rotation = getRotation();
		float accelFactor = m_physicTimer.getFullWaitedDuration().asSeconds();

		//calculate the new speed with the acceleration
		m_speed += accelFactor * m_acceleration;
		if (m_speed > m_maxSpeed)
		{
			m_speed = m_maxSpeed;
			//std::cout<< "max attained\n";
		}
		else if (m_speed < -m_maxSpeed)
		{
			m_speed = -m_maxSpeed;
			//std::cout<< "min attained\n";
		}

		sf::Vector2f posOffset(
			m_speed * accelFactor * std::cos(rotation * M_PI / 180), m_speed * accelFactor * std::sin(rotation * M_PI / 180));

		//calculate the new position with the speed
		move(posOffset);

		//collisions tests
		bool collided = false;
		collision::LineHitBox lineBox;
		for (Map::iterator it = map.begin(); it != map.end() && !collided; it++)
		{
			collided = collision::collision(getHitBox(), it->getHitBox(), lineBox);
		}
		if (collided)
		{
			move(-posOffset); //return to position before collision
			posOffset = collision::bounceVector(
				posOffset, collision::normale(lineBox, getPosition()));

			move(posOffset);
			setRotation(angle(posOffset));
			m_speed /= 4;
			std::cout << "collision " << getPosition().x << " ; " << getPosition().y << '\n';
		}

		sf::FloatRect rect = m_sprite.getLocalBounds();
		sf::Vector2f transformedTopLeft = m_sprite.getTransform() * sf::Vector2f(rect.left, rect.top);
		sf::Vector2f transformedTopRight = m_sprite.getTransform() * sf::Vector2f(rect.left + rect.width, rect.top);
		sf::Vector2f transformedBottomLeft = m_sprite.getTransform() * sf::Vector2f(rect.left, rect.top + rect.height);
		sf::Vector2f transformedBottomRight = m_sprite.getTransform() * sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
		sf::Vector2f transformedMidRight = m_sprite.getTransform() * sf::Vector2f(rect.left + rect.width, rect.top + rect.height / 2);
		sf::Vector2f transformedMidLeft = m_sprite.getTransform() * sf::Vector2f(rect.left, rect.top + rect.height / 2);

		//std::cout << "ttl x:" << transformedTopLeft.x << " y:" << transformedTopLeft.y << "\n";
		//std::cout << "ttr x:" << transformedTopRight.x << " y:" << transformedTopRight.y << "\n";
		//std::cout << "tbl x:" << transformedBottomLeft.x << " y:" << transformedBottomLeft.y << "\n";
		//std::cout << "tbr x:" << transformedBottomRight.x << " y:" << transformedBottomRight.y << "\n";
		// front / back
		m_line_back_to_front.update(transformedMidLeft, transformedMidRight);
		// front sides
		m_line_front_left_to_right.update(transformedTopRight, transformedBottomRight);
		// back sides
		m_line_back_left_to_right.update(transformedTopLeft, transformedBottomLeft);

		// diagonal
		m_line_diagonal1.update(transformedTopLeft, transformedBottomRight);
		m_line_diagonal2.update(transformedTopRight, transformedBottomLeft);

		m_acceleration = 0;
		m_physicTimer.restart();
	}
}

float Car::norm(const sf::Vector2f &v) const
{
	return std::sqrt((v.y * v.y) + (v.x * v.x));
}

float Car::angle(const sf::Vector2f &v) const
{
	float signY = v.y >= 0 ? 1 : -1;

	float arcTan = std::atan(v.y / v.x >= 0 ? v.y / v.x : -v.y / v.x);

	float addPi = v.x < 0 ? M_PI / 2. : 0;

	float ans = signY * (arcTan + addPi);

	return ans * 180 / M_PI;
}

float Car::getSpeed() const
{
	return m_speed;
}

collision::CircleHitBox Car::getHitBox() const
{
	return collision::CircleHitBox(getPosition(), m_hitBoxRadius);
}
