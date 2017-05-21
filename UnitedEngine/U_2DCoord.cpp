#include "U_2DCoord.h"
#include <cmath>

U_2DCoord::U_2DCoord() : x(0), y(0) {}

U_2DCoord::U_2DCoord(const U_2DCoord& coord1, const U_2DCoord& coord2)
{
    x = coord2.x-coord1.x;
    y = coord2.y-coord1.y;
}

U_2DCoord::U_2DCoord(double cx, double cy) : x(cx), y(cy) {}

void U_2DCoord::move2D(U_2DCoord& vec)
{
    this->x += vec.x;
    this->y += vec.y;
}

double U_2DCoord::getNorm() const
{
    return sqrt(x*x+y*y);
}

U_2DCoord operator/(const U_2DCoord& c, double d)
{
    return U_2DCoord(c.x/d, c.y/d);
}

U_2DCoord operator-(const U_2DCoord& c1, const U_2DCoord& c2)
{
    return U_2DCoord(c1.x-c2.x, c1.y-c2.y);
}

U_2DCoord operator-(const U_2DCoord& c1)
{
    return U_2DCoord(-c1.x, -c1.y);
}

U_2DCoord operator*(const U_2DCoord& c, double d)
{
    return U_2DCoord(c.x*d, c.y*d);
}
U_2DCoord operator*(double d, U_2DCoord c)
{
    return U_2DCoord(c.x*d, c.y*d);
}

U_2DCoord operator+(const U_2DCoord c1, const U_2DCoord c2)
{
    return U_2DCoord(c1.x+c2.x, c1.y+c2.y);
}

U_2DCoord operator+(const U_2DCoord& c1, const U_2DCoord& c2)
{
    return U_2DCoord(c1.x+c2.x, c1.y+c2.y);
}
