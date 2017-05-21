#ifndef EXPLOSIONPROVIDER_HPP_INCLUDED
#define EXPLOSIONPROVIDER_HPP_INCLUDED

#include "Explosion.hpp"

class ExplosionProvider
{
public:
    static Explosion getBase(U_2DCoord pos);
    static Explosion getHit(U_2DCoord pos, float angle);
    static Explosion getThrough(U_2DCoord pos, float angle);

    static Explosion getBig(U_2DCoord pos);
    static Explosion getBigFast(U_2DCoord pos);
    static Explosion getBigSlow(U_2DCoord pos);
};

#endif // EXPLOSIONPROVIDER_HPP_INCLUDED
