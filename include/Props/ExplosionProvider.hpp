#ifndef EXPLOSIONPROVIDER_HPP_INCLUDED
#define EXPLOSIONPROVIDER_HPP_INCLUDED

#include "Explosion.hpp"

class ExplosionProvider
{
public:
    static Explosion* getBase(Vec2 pos, bool isTrace=false);
    static Explosion* getHit(Vec2 pos, float angle, bool isTrace=false);
    static Explosion* getThrough(Vec2 pos, float angle, bool isTrace=false);
    static Explosion* getBigThrough(Vec2 pos, float angle, bool isTrace=false);

    static Explosion* getBig(Vec2 pos, bool isTrace=false);
    static Explosion* getBigFast(Vec2 pos, bool isTrace=false);
    static Explosion* getBigSlow(Vec2 pos, bool isTrace=false);
    static Explosion* getClose(Vec2 pos, float angle, bool isTrace=false);
};

#endif // EXPLOSIONPROVIDER_HPP_INCLUDED
