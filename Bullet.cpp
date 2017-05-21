#include "Bullet.hpp"

#include "GameWorld.hpp"
#include "ExplosionProvider.hpp"
#include <cmath>
#include <iostream>

sf::Texture Bullet::__texture;

Bullet::Bullet()
{

}

Bullet::Bullet(double angle, double speed, double damage, int penetration):
    __ownVertexArray(sf::Quads, 4)
{
    __angle  = angle;
    __speed  = speed;
    __damage = damage;
    __done   = false;
    __penetration = penetration;
}

void Bullet::init(U_2DCoord pos, double angle)
{
    pos.x += __offset*cos(angle+PI/2.0f);
    pos.y += __offset*sin(angle+PI/2.0f);
    __body.setPosition(pos);
    __angle += angle;
    __vx     = __speed*cos(__angle);
    __vy     = __speed*sin(__angle);

    float length = 30;
    float rectA  = PI/40.0;

    /// Can be optimized : half of these vectors are just opposites of other half
    /// I understand myself
    __ownVertexArray[0].position = sf::Vector2f(pos.x+length*cos(__angle+rectA)   , pos.y+length*sin(__angle+rectA));
    __ownVertexArray[1].position = sf::Vector2f(pos.x+length*cos(__angle+PI-rectA), pos.y+length*sin(__angle+PI-rectA));
    __ownVertexArray[2].position = sf::Vector2f(pos.x+length*cos(__angle+PI+rectA), pos.y+length*sin(__angle+PI+rectA));
    __ownVertexArray[3].position = sf::Vector2f(pos.x+length*cos(__angle-rectA)   , pos.y+length*sin(__angle-rectA));
    __ownVertexArray[0].texCoords = sf::Vector2f(0, 0);
    __ownVertexArray[1].texCoords = sf::Vector2f(0, 500);
    __ownVertexArray[2].texCoords = sf::Vector2f(235, 500);
    __ownVertexArray[3].texCoords = sf::Vector2f(235, 0);

    /// Initialize traveled distance
    __distance = 0.0;
}

void Bullet::update(GameWorld& world)
{
    /// Update position
    U_2DCoord v(__vx, __vy);
    U_2DCoord pos = __body.getPosition();
    __body.move2D(v);
    __distance += __speed;

    /// Move the vertexArray
    for (int i(0); i<4; ++i)
    {
        __ownVertexArray[i].position.x += __vx;
        __ownVertexArray[i].position.y += __vy;
    }

    /// Check collisions with surrounding enemies
    GridCell* bodies = world.getBodiesAt(getCoord());
    if (bodies)
    {
        for (int i(0); i<bodies->_maxIndex; ++i)
        {
            U2DBody_ptr body    = bodies->_bodies[i];
            WorldEntity* entity = body->getEntity();

            if (collidesWith(entity) && entity->isEnemy() && __penetration>=0)
            {
                float recoilForce = __penetration==0?10:5;
                __penetration--;

                body  ->accelerate2D(v*recoilForce);
                entity->addLife(-__damage);
                entity->resetTime();
                pos.move2D(v);

                world.addExplosion(ExplosionProvider::getHit(pos, __angle+PI), true);
                world.addExplosion(ExplosionProvider::getHit(pos, __angle   ), true);
                world.addExplosion(ExplosionProvider::getBase(pos));
                if (__penetration>0)
                    world.addExplosion(ExplosionProvider::getThrough(pos, __angle), true);
            }
        }

        __done = __penetration<0;
    }
}

void Bullet::render()
{
    float minRenderDist = 100.0f;
    if (__distance > minRenderDist)
    {
        __vertexArray.append(__ownVertexArray[0]);
        __vertexArray.append(__ownVertexArray[1]);
        __vertexArray.append(__ownVertexArray[2]);
        __vertexArray.append(__ownVertexArray[3]);
    }
}

bool Bullet::collidesWith(WorldEntity* entity) const
{
    float distFactor = 1.0f;
    U_2DCoord vec(entity->getCoord(), getCoord());

    return vec.getNorm() < distFactor*CELL_SIZE;
}

void Bullet::loadTexture()
{
    __texture.loadFromFile("data/textures/bullet.png");
}
