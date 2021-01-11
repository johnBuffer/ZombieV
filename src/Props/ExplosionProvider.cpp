#include "Props/ExplosionProvider.hpp"
#include "System/Config.hpp"

/// A simple Explosion builder to avoid code repetition
Explosion* ExplosionProvider::getBase(const Vec2& pos, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 360.0f, 0.0f, 3.0f, SCALE*50.0f, 15);
    e->setDecrease(0.05f);
    e->setTrace(isTrace);

    return e;
}

Explosion* ExplosionProvider::getHit(const Vec2& pos, float angle, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 170.0f, angle, 10.0f, SCALE*5.0f, 15);
    e->setDecrease(0.05f);
    e->setTrace(isTrace);

    return e;
}

Explosion* ExplosionProvider::getThrough(const Vec2& pos, float angle, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 1.0f, angle, 15.0f, SCALE*3.0f, 50);
    e->setDecrease(0.035f);
    e->setTrace(isTrace);

    return e;
}

Explosion* ExplosionProvider::getBigThrough(const Vec2& pos, float angle, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 1.0f, angle, 10.0f, SCALE*30.0f, 10);
    e->setDecrease(0.05f);
    e->setTrace(isTrace);

    return e;
}

Explosion* ExplosionProvider::getBig(const Vec2& pos, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 360.0f, 0.0f, 10.0f, SCALE*75.0f, 7);
    e->setDecrease(0.1f);
    e->setTrace(isTrace);

    return e;
}

Explosion* ExplosionProvider::getBigFast(const Vec2& pos, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 360.0f, 0.0f, 10.0f, SCALE*35.0f, 80);
    e->setDecrease(0.07f);
    e->setTrace(isTrace);

    return e;
}

Explosion* ExplosionProvider::getBigSlow(const Vec2& pos, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 360.0f, 0.0f, 2.0f, SCALE*70.0f, 15);
    e->setDecrease(0.025f);
    e->setTrace(isTrace);

    return e;
}

Explosion* ExplosionProvider::getClose(const Vec2& pos, float angle, bool isTrace)
{
    Explosion* e = Explosion::add(pos.x, pos.y, 170.0f, angle+PI, 20.0f, SCALE*10.0f, 50);
    e->setDecrease(0.1f);
    e->setTrace(isTrace);

    return e;
}
