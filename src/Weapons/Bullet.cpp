#include "Weapons/Bullet.hpp"

#include "System/GameWorld.hpp"
#include "Props/ExplosionProvider.hpp"
#include <cmath>
#include <iostream>
#include "System/GameRender.hpp"
#include "Props/Guts.hpp"
#include <cmath>

size_t Bullet::_textureID;

Bullet::Bullet()
{
}

Bullet::Bullet(float angle, float speed, float damage, int penetration) :
    StandardEntity(0.0f, 0.0f, angle),
    _speed(speed),
    _v(_speed, _speed),
    _ownVertexArray(sf::Quads, 4)
{
    _damage = damage;
    _done   = false;
    _penetration = penetration;
    _type = EntityTypes::BULLET;
    _new = true;
    _drawCount = rand()%4;
    _impact = 10.0f;
}

Bullet::Bullet(const Bullet& bullet)
{
}

Bullet::~Bullet()
{
}

void Bullet::kill(GameWorld* world)
{
    this->remove();
}

void Bullet::init(const Vec2& pos, float angle)
{
    m_coord = pos;
    _angle += angle;

    const float length = 40.0f;
    const float rectA  = PI/length;

    /// Can be optimized : half of these vectors are just opposites of other half
    /// I understand myself
    _ownVertexArray[0].position = sf::Vector2f(pos.x+length*cos(_angle+rectA)   , pos.y+length*sin(_angle+rectA));
    _ownVertexArray[1].position = sf::Vector2f(pos.x+length*cos(_angle+PI-rectA), pos.y+length*sin(_angle+PI-rectA));
    _ownVertexArray[2].position = sf::Vector2f(pos.x+length*cos(_angle+PI+rectA), pos.y+length*sin(_angle+PI+rectA));
    _ownVertexArray[3].position = sf::Vector2f(pos.x+length*cos(_angle-rectA)   , pos.y+length*sin(_angle-rectA));
    _ownVertexArray[0].texCoords = sf::Vector2f(0, 0);
    _ownVertexArray[1].texCoords = sf::Vector2f(0, 500);
    _ownVertexArray[2].texCoords = sf::Vector2f(235, 500);
    _ownVertexArray[3].texCoords = sf::Vector2f(235, 0);

    /// Initialize traveled distance
    _distance = 0.0;
    _v.x *= cos(_angle);
    _v.y *= sin(_angle);
}

void Bullet::update(GameWorld& world)
{
    //std::cout << "Update Bullet v:" << _v.x << std::endl;

    /// Update position
    m_coord.move2D(_v);
    _distance += _speed;

    /// Move the vertexArray
    for (int i(0); i<4; ++i)
    {
        _ownVertexArray[i].position.x += _v.x;
        _ownVertexArray[i].position.y += _v.y;
    }

    /// Check collisions with surrounding enemies
    GridCell* bodies = world.getBodiesAt(getCoord());
    if (bodies)
    {
        for (int i(bodies->_maxIndex); i--;)
        {
            U2DBody_ptr body    = GameWorld::getBodyByID(bodies->_bodiesID[i]);
            WorldEntity* entity = body->getEntity();

            /// if collides with entity, update penetration and damage
            if (entity)
            {
                if (collidesWith(entity) && _penetration>=0 && entity->getType() == EntityTypes::ZOMBIE)
                {
                    _recoilForce = (!(_penetration--))?_impact:_impact*0.5f;
                    entity->hit(this, &world);
                    _new = false;
                }
            }
        }

        _done = _penetration<0;
    }

    _done = _done || !world.isInLevelBounds(m_coord);

    //std::cout << "Update Bullet done" << std::endl;
}

void Bullet::render()
{
    float minRenderDist = 100.0f;
    if (_distance > minRenderDist && GameRender::isVisible(this) && !(_drawCount++%4))
    {
        GameRender::addQuad(_textureID, _ownVertexArray, RenderLayer::RENDER);
        GameRender::addQuad(_textureID, _ownVertexArray, RenderLayer::BLOOM);

        Light light;
        light.position = getCoord();
        light.color    = sf::Color(255, 127, 0);
        light.radius   = 20;

        GameRender::getLightEngine().addTempLight(light);
    }
}

bool Bullet::collidesWith(WorldEntity* entity) const
{
    float distFactor = 1.0f;
    Vec2 vec(entity->getCoord(), m_coord);

    return vec.getNorm2() < distFactor*CELL_SIZE*CELL_SIZE;
}

void Bullet::init()
{
    _textureID = GameRender::registerTexture("data/textures/bullet.png");
}


Vec2 Bullet::getImpactForce() const
{
    Vec2 result(_v.x*_recoilForce, _v.y*_recoilForce);

    return result;
}
