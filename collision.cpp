#include "collision.hpp"
#include "RoadBox.hpp"
#include <cmath> //sqrt needed
#include <cassert>

#include <iostream>

namespace collision
{

RectHitBox::RectHitBox()
{
	orientation = 0;
}

RectHitBox::RectHitBox(const sf::FloatRect &rect1, float orient1)
{
	rect = rect1;
	orientation = orient1;
}

LineHitBox::LineHitBox()
{
	p1 = sf::Vector2f(0, 0);
	p2 = p1;
}

LineHitBox::LineHitBox(const sf::Vector2f &point1, const sf::Vector2f &point2)
{
	p1 = point1;
	p2 = point2;
}

void LineHitBox::move(const sf::Vector2f &point2)
{
	p1 += point2;
	p2 += point2;
}

CircleHitBox::CircleHitBox()
{
	p = sf::Vector2f(0, 0);
	rayon = 1;
}

CircleHitBox::CircleHitBox(const sf::Vector2f &point, float rayon1)
{
	p = point;
	rayon = rayon1;
}

bool collision(const RectHitBox &rectBox1, const RectHitBox &rectBox2)
{
	sf::FloatRect rect1 = rectBox1.rect, rect2 = rectBox2.rect;

	/*int centre1_x = rect1.left + rect1.width/ 2;
		int centre1_y = rect1.top + rect1.height/ 2;
		int centre2_x = rect2.left + rect2.width/ 2;
		int centre2_y = rect2.top + rect2.height/ 2;*/

	int dis2centre = (rect1.left - rect2.left) * (rect1.left - rect2.left) + (rect1.top - rect2.top) * (rect1.top - rect2.top); //Distance entre deux centre

	int rayon1 = ((rect1.height * rect1.height) + (rect1.width * rect1.width)) / 2;
	int rayon2 = ((rect2.height * rect2.height) + (rect2.width * rect2.width)) / 2;
	return (dis2centre < (rayon1 + rayon2) * (rayon1 + rayon2));
}

bool collision(const sf::Vector2f &point, const CircleHitBox &cercleBox)
{
	sf::Vector2f C(cercleBox.p);
	float x = point.x, y = point.y;
	float rayon = cercleBox.rayon;

	float d2 = (x - C.x) * (x - C.x) + (y - C.y) * (y - C.y);
	if (d2 > rayon * rayon)
		return false;
	else
		return true;
}

bool collisionAsInfiniteLine(const CircleHitBox &cercleBox, const LineHitBox &lineBox)
{
	sf::Vector2f A(lineBox.p1), B(lineBox.p2), C(cercleBox.p);
	sf::Vector2f u;
	u.x = B.x - A.x;
	u.y = B.y - A.y;

	sf::Vector2f AC;
	AC.x = C.x - A.x;
	AC.y = C.y - A.y;

	float numerateur = u.x * AC.y - u.y * AC.x;
	if (numerateur < 0)
	{
		numerateur = -numerateur;
	}

	float denominateur = std::sqrt(u.x * u.x + u.y * u.y);
	float CI = numerateur / denominateur;
	if (CI < cercleBox.rayon)
	{
		return true;
	}
	return false;
}

bool collision(const CircleHitBox &cercleBox, const LineHitBox &lineBox)
{
	sf::Vector2f A(lineBox.p1), B(lineBox.p2), C(cercleBox.p);

	if (!collisionAsInfiniteLine(cercleBox, lineBox))
	{
		return false;
	}

	sf::Vector2f AB, AC, BC;
	AB.x = B.x - A.x;
	AB.y = B.y - A.y;
	AC.x = C.x - A.x;
	AC.y = C.y - A.y;
	BC.x = C.x - B.x;
	BC.y = C.y - B.y;
	float pscal1 = AB.x * AC.x + AB.y * AC.y;		// produit scalaire
	float pscal2 = (-AB.x) * BC.x + (-AB.y) * BC.y; // produit scalaire
	if (pscal1 >= 0 && pscal2 >= 0)
		return true; // I entre A et B, ok.
	// dernière possibilité, A ou B dans le cercle
	if (collision(A, cercleBox))
		return true;
	if (collision(B, cercleBox))
		return true;
	return false;
}

bool collision(const CircleHitBox &cercleBox1, const CircleHitBox &cercleBox2)
{
	sf::Vector2f C1(cercleBox1.p), C2(cercleBox2.p);
	float d2 = (C1.x - C2.x) * (C1.x - C2.x) + (C1.y - C2.y) * (C1.y - C2.y);
	if (d2 > (cercleBox1.rayon + cercleBox2.rayon) * (cercleBox1.rayon + cercleBox2.rayon))
		return false;
	else
		return true;
}

bool collision(const CircleHitBox &circleBox, const RoadBox &roadBox, LineHitBox &lineBox)
{
	bool collided = false;
	const std::vector<collision::LineHitBox> &hitBox = roadBox.getLineArray();

	for (unsigned int i = 0; i < hitBox.size() && !collided; i++)
	{
		collided = collision(circleBox, hitBox[i]);
		if (collided)
		{
			//std::cout<< "collision\n";
			lineBox = hitBox[i]; //save which line made the collision
		}
	}

	return collided;
}

sf::Vector2f projection(const LineHitBox &lineBox, const sf::Vector2f &p)

{
	sf::Vector2f A(lineBox.p1), B(lineBox.p2), C(p);
	sf::Vector2f u, AC;

	u.x = B.x - A.x;

	u.y = B.y - A.y;

	AC.x = C.x - A.x;

	AC.y = C.y - A.y;

	float ti = (u.x * AC.x + u.y * AC.y) / (u.x * u.x + u.y * u.y);

	sf::Vector2f I;

	I.x = A.x + ti * u.x;

	I.y = A.y + ti * u.y;

	return I;
}

sf::Vector2f normale(const LineHitBox &lineBox, const sf::Vector2f &p)

{
	sf::Vector2f A(lineBox.p1), B(lineBox.p2), C(p);
	sf::Vector2f AC, u, N;

	u.x = B.x - A.x;

	u.y = B.y - A.y;

	AC.x = C.x - A.x;

	AC.y = C.y - A.y;

	float parenthesis = u.x * AC.y - u.y * AC.x; // calcul une fois pour les deux

	N.x = -u.y * (parenthesis);

	N.y = u.x * (parenthesis);

	// normalisons

	float norme = std::sqrt(N.x * N.x + N.y * N.y);
	//std::cout<< "norme = "<<norme<< '\n';
	assert(norme != 0);

	N.x /= norme;

	N.y /= norme;

	return N;
}

bool get_line_intersection(LineHitBox lineA, LineHitBox lineB, float *i_x, float *i_y)
{
	float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	s10_x = lineA.p2.x - lineA.p1.x;
	s10_y = lineA.p2.y - lineA.p1.y;
	s32_x = lineB.p2.x - lineB.p1.x;
	s32_y = lineB.p2.y - lineB.p1.y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
		return false; // Collinear
	bool denomPositive = denom > 0;

	s02_x = lineA.p1.x - lineB.p1.x;
	s02_y = lineA.p1.y - lineB.p1.y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	if ((s_numer < 0) == denomPositive)
		return false; // No collision

	t_numer = s32_x * s02_y - s32_y * s02_x;
	if ((t_numer < 0) == denomPositive)
		return false; // No collision

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
		return false; // No collision
	// Collision detected
	t = t_numer / denom;
	if (i_x != NULL)
		*i_x = lineA.p1.x + (t * s10_x);
	if (i_y != NULL)
		*i_y = lineA.p1.y + (t * s10_y);

	return true;
}

sf::Vector2f minHitPoint(const LineHitBox &ray, const std::vector<collision::LineHitBox> &roadLines)
{
	float crrMinDist = std::numeric_limits<float>::max();
	sf::Vector2f result;
	float intersect_x;
	float intersect_y;
	for (unsigned int i = 0; i < roadLines.size(); i++)
	{
		if (get_line_intersection(ray, roadLines[i], &intersect_x, &intersect_y))
		{
			//std::cout << "Found intersection at x:" << intersect_x << " y:" << intersect_y << " road : " << i << "\n";
			float dist = calcDistance(ray.p1, sf::Vector2f(intersect_x, intersect_y));
			if (dist < crrMinDist)
			{
				result.x = intersect_x;
				result.y = intersect_y;
				crrMinDist = dist;
			}
		}
	}
	return result;
}

float calcDistance(const sf::Vector2f pointA, const sf::Vector2f pointB)
{
	return std::sqrt(std::pow(pointA.x - pointB.x, 2.0) + std::pow(pointA.y - pointB.y, 2.0));
}

sf::Vector2f bounceVector(const sf::Vector2f &v, const sf::Vector2f &N)

{

	sf::Vector2f v2;

	float pscal = (v.x * N.x + v.y * N.y);

	v2.x = v.x - 2 * pscal * N.x;

	v2.y = v.y - 2 * pscal * N.y;

	return v2;
}

} //namespace collision
