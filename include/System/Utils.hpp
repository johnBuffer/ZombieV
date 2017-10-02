#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "UnitedEngine/Vec2.h"

float getRandFloat(float min, float max);
float getRandomAngle();
float getRandomAngle(float min, float max);
float getAngleFromVec(const sf::Vector2i& v);
float sign(float f);
Vec2  transformVec(const Vec2& v, float r, Vec2 t);

#endif // UTILS_HPP_INCLUDED
