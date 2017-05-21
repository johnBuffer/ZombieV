#include "ExplosionProvider.hpp"

Explosion ExplosionProvider::getBase(U_2DCoord pos)
{
    Explosion e(pos.x, pos.y, 360, 0.0f, 4.0f, 50.0f, 15);
    e.setDecrease(0.1);

    return e;
}

Explosion ExplosionProvider::getHit(U_2DCoord pos, float angle)
{
    Explosion e(pos.x, pos.y, 170, angle, 10.0f, 5.0f, 15);
    e.setDecrease(0.05);

    return e;
}

Explosion ExplosionProvider::getThrough(U_2DCoord pos, float angle)
{
    Explosion e(pos.x, pos.y, 5, angle, 20.0f, 7.0f, 10);
    e.setDecrease(0.035);

    return e;
}

Explosion ExplosionProvider::getBig(U_2DCoord pos)
{
    Explosion e(pos.x, pos.y, 360, 0.0f, 6.0f, 60.0f, 7);
    e.setDecrease(0.05);

    return e;
}

Explosion ExplosionProvider::getBigFast(U_2DCoord pos)
{
    Explosion e(pos.x, pos.y, 360, 0.0f, 10.0f, 35.0f, 80);
    e.setDecrease(0.07);

    return e;
}

Explosion ExplosionProvider::getBigSlow(U_2DCoord pos)
{
    Explosion e(pos.x, pos.y, 360, 0.0f, 2.0f, 70.0f, 15);
    e.setDecrease(0.025);

    return e;
}
