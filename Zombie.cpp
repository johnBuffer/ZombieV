#include "Zombie.hpp"
#include "GameWorld.hpp"
#include "GameRender.hpp"

#include <iostream>

size_t      Zombie::_moveTextureID;
size_t      Zombie::_attackTextureID;
Animation   Zombie::_moveAnimation(3, 6, 288, 311, 17, 20);
Animation   Zombie::_attackAnimation(3, 3, 954/3, 882/3, 9, 20);

Zombie::Zombie(float x, float y):
    WorldEntity(x, y, 0.0f),
    _vertexArray(sf::VertexArray(sf::Quads, 4))
{
    _speed = 150;
    _life  = 100;
    _done  = false;

    _currentAnimation = _moveAnimation;

    _time = rand()%100;
    _type = EntityTypes::ZOMBIE;

    _currentState = IDLE;

    Zombie::add(this);
}

Zombie::~Zombie()
{
    Zombie::remove(this);
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
        Vec2 _targetPos = _target->getCoord();
        float vx = _targetPos.x - _body.getPosition().x;
        float vy = _targetPos.y - _body.getPosition().y;

        Vec2 direction(cos(_angle), sin(_angle));
        Vec2 directionNormal(-direction.y, direction.x);

        float dot2 = vx*directionNormal.x + vy*directionNormal.y;
        float coeff = 0.025f;

        if (dot2 > 0)
        {
            _angle += coeff;
        }
        else
        {
            _angle -= coeff;
        }

        //vx /= dist;
        //vy /= dist;

        if (_currentState == MOVING)
        {
            float speed = 10;
            getBody().accelerate2D(Vec2(speed*direction.x, speed*direction.y));
        }
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

    if (_currentAnimation.isDone())
    {
        _currentAnimation = _moveAnimation;
        _currentState = MOVING;
    }
}

void Zombie::render()
{
    if (GameRender::isVisible(this))
    {
        float x = _body.getPosition().x;
        float y = _body.getPosition().y;

        GraphicUtils::initQuad(_vertexArray, sf::Vector2f(288, 311), sf::Vector2f(144, 155), SCALE*0.25);
        sf::VertexArray& vertices(_vertexArray);
        GraphicUtils::transform(vertices, sf::Vector2f(x, y), _angle);

        _currentAnimation.applyOnQuad(vertices, _time);

        GameRender::addQuad(_currentAnimation.getTexture(), vertices, RenderLayer::RENDER, true);
        GraphicUtils::createEntityShadow(this);
    }
}

void Zombie::init()
{
    _moveAnimation.setCenter(sf::Vector2f(90, 168));
    _moveTextureID = GameRender::registerTexture("data/textures/zombie/zombie_move.png");
    _attackTextureID = GameRender::registerTexture("data/textures/zombie/zombie_attack.png");

    _moveAnimation.setTextureID(_moveTextureID);
    _attackAnimation.setTextureID(_attackTextureID);

    _head = nullptr;
}

void Zombie::hit(WorldEntity* entity, GameWorld* gameWorld)
{
    switch(entity->getType())
    {
        case(EntityTypes::BULLET) :
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            getBody().accelerate2D(bullet->getImpactForce());
            addLife(-bullet->getDamage());
            resetTime();
            break;
        }
        case(EntityTypes::HUNTER) :
        {
            if (_currentState != ATTACKING)
            {
                _currentState     = ATTACKING;
                _currentAnimation = _attackAnimation;
                _currentAnimation.resetTime(_time);
            }
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
