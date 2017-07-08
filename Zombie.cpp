#include "Zombie.hpp"
#include "GameWorld.hpp"
#include "GameRender.hpp"

#include <iostream>

size_t      Zombie::_moveTextureID;
Animation   Zombie::_moveAnimation(3, 6, 288, 311, 17, 20);

Zombie::Zombie(float x, float y):
    WorldEntity(x, y, 0.0f),
    _vertexArray(sf::VertexArray(sf::Quads, 4))
{
    _speed = 150;
    _life  = 1000;
    _done  = false;

    _currentAnimation = _moveAnimation;

    int c=rand()%50+200;
    _color=sf::Color(c, c, c);

    _time = rand()%100;
    _type = EntityTypes::ZOMBIE;
}

void Zombie::setTarget(WorldEntity* target)
{
    _target = target;
}

void Zombie::update(GameWorld& world)
{
    _time += DT;

    if (_target)
    {
        float vx = _target->getCoord().x - _body.getPosition().x;
        float vy = _target->getCoord().y - _body.getPosition().y;
        float dist = sqrt(vx*vx+vy*vy);
        _angle = acos(vx/dist)+PI;
        _angle = vy < 0 ? -_angle : _angle;

        vx /= dist;
        vy /= dist;

        float speed = 10;

        getBody().accelerate2D(Vec2(speed*vx, speed*vy));
    }

    if (_life<0)
    {
        const Vec2 coord = getCoord();
        world.addEntity(ExplosionProvider::getBig(coord, true));
        world.addEntity(ExplosionProvider::getBigFast(coord));
        world.addEntity(ExplosionProvider::getBase(coord));
        world.removeBody(&_body);
        _done = true;
    }
}

void Zombie::render()
{
    if (GameRender::isVisible(this))
    {
        float x = _body.getPosition().x;
        float y = _body.getPosition().y;

        GraphicUtils::initQuad(_vertexArray, sf::Vector2f(288, 311), sf::Vector2f(144, 155), 0.75*0.25);
        sf::VertexArray& vertices(_vertexArray);
        GraphicUtils::transform(vertices, sf::Vector2f(x, y), _angle+PI);

        _currentAnimation.applyOnQuad(vertices, _time);

        GameRender::addQuad(_moveTextureID, vertices, RenderLayer::RENDER);
        GraphicUtils::createEntityShadow(this);
    }
}

void Zombie::init()
{
    _moveAnimation.setCenter(sf::Vector2f(90, 168));
    _moveTextureID = GameRender::registerTexture("data/textures/zombie/zombie_move.png");

}

void Zombie::hit(WorldEntity* entity, GameWorld* gameWorld)
{
    switch(entity->getType())
    {
        case(EntityTypes::BULLET):
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            getBody().accelerate2D(bullet->getImpactForce());
            addLife(-bullet->getDamage());
            resetTime();
            break;
        }
        default:
        {
            break;
        }
    }
}

void Zombie::initPhysics(GameWorld* world)
{
    world->addBody(&_body);
}
