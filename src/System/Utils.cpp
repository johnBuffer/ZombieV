#include "System/Utils.hpp"

#include <cmath>
#include <sstream>

float getRand()
{
	constexpr uint32_t range = 10000;
	return (rand() % range) / static_cast<float>(range);
}

float getRandUnder(float max)
{
	return getRand() * max;
}

int getRandInt(int min, int max)
{
    return min+rand()%(max-min+1);
}

float getRandFloat(float min, float max)
{
	return min + (max - min) * getRand();
}

float getRandomAngle()
{
    return 0.0f;
}

float getRandomAngle(float min, float max)
{
	return min + (max - min)*getRand();
}

// Whaaat ? should be a v2f
float getAngleFromVec(const sf::Vector2i& v)
{
    float result;
    const float ax = static_cast<float>(v.x);
    const float ay = static_cast<float>(v.y);
    const float anorm = sqrt(ax*ax+ay*ay);

    result = acos(ax/anorm);

    result = ay < 0 ? -result : result;

    return result;
}

float sign(float f)
{
    return f<0.0f?-1.f:1.0f;
}

Vec2 transformVec(const Vec2& v, float r, Vec2 t)
{
    float cosa = cos(r);
    float sina = sin(r);

    return Vec2(v.x*cosa-v.y*sina+t.x, v.x*sina+v.y*cosa+t.y);
}

std::string intToString(int i)
{
    std::stringstream sstr;
    sstr << i;
    return sstr.str();
}

