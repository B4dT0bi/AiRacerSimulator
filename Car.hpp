/**
* A Car shall be drawable and transformable. It can collide with other objects.
**/
#ifndef DEF_CAR_HPP
#define DEF_CAR_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Timer.hpp"
#include "collision.hpp"
#include "Line.hpp"

namespace sf
{
class Texture;
}

class Map;

class Car : public sf::Drawable, public sf::Transformable
{
  public:
	Car(sf::Texture &tex, float maxSpeed = 100);

	void accelerate(float accel);

	void rotate(float rot);

	void apply_physics(Map &map);

	collision::CircleHitBox getHitBox() const;

	float getSpeed() const;

	void toggleShowHitbox();
	void toggleShowDistanceLines();

  protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  private:
	//calculate the norm and angle of a vector
	float norm(const sf::Vector2f &v) const;
	float angle(const sf::Vector2f &v) const;

	//buffers used in physics
	float m_acceleration;
	float m_rotation;

	sf::Sprite m_sprite;
	
	Timer m_physicTimer; //timer that permit the physics to apply at every frame

	float m_maxSpeed;
	float m_speed;

	float m_hitBoxRadius;
};

#endif
