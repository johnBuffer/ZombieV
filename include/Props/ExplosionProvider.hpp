#ifndef EXPLOSIONPROVIDER_HPP_INCLUDED
#define EXPLOSIONPROVIDER_HPP_INCLUDED

#include "Explosion.hpp"

class ExplosionProvider
{
public:
    static Explosion* getBase(const Vec2& pos, bool isTrace=false);
    static Explosion* getHit(const Vec2& pos, float angle, bool isTrace=false);
    static Explosion* getThrough(const Vec2& pos, float angle, bool isTrace=false);
    static Explosion* getBigThrough(const Vec2& pos, float angle, bool isTrace=false);

    static Explosion* getBig(const Vec2& pos, bool isTrace=false);
    static Explosion* getBigFast(const Vec2& pos, bool isTrace=false);
    static Explosion* getBigSlow(const Vec2& pos, bool isTrace=false);
    static Explosion* getClose(const Vec2& pos, float angle, bool isTrace=false);
};

#endif // EXPLOSIONPROVIDER_HPP_INCLUDED
