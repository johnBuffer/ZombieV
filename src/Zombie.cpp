#include "Zombie.hpp"
#include "System/GameWorld.hpp"
#include "System/GameRender.hpp"
#include "Props/Guts.hpp"
#include "Bot.hpp"


#include <iostream>

size_t      Zombie::_moveTextureID;
size_t      Zombie::_attackTextureID;
Animation   Zombie::_moveAnimation(3, 6, 288, 311, 17, 20);
Animation   Zombie::_attackAnimation(3, 3, 954/3, 882/3, 9, 20);

Zombie::Zombie(float x, float y) :
    StandardEntity(x, y, 0.0f),
    _vertexArray(sf::VertexArray(sf::Quads, 4))
{
    _speed = 150;
    _life  = 100;
    _done  = false;

    _currentAnimation = _moveAnimation;

    _time = rand()%100;
    _type = EntityTypes::ZOMBIE;

    _currentState = IDLE;
    _marked = false;
    _target = nullptr;
}

Zombie::~Zombie()
{
    //std::cout << "Suicide" << std::endl;
}

void Zombie::kill()
{
    this->remove();
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
        Vec2 vTarget(_target->getCoord(), getCoord());
        Vec2 direction(cos(_angle), sin(_angle));
        Vec2 directionNormal(-direction.y, direction.x);

        float dist = vTarget.getNorm();
        float vx = vTarget.x/dist;
        float vy = vTarget.y/dist;

        float dot2 = vx*directionNormal.x + vy*directionNormal.y;
        float coeff = 0.04f;

        float absDot = std::abs(dot2);
        coeff *= absDot;

        _angle += dot2>0?-coeff:coeff;

        if (_currentState == MOVING)
        {
            float speed = 10;
            getBody().accelerate2D(Vec2(speed*direction.x, speed*direction.y));
        }
        else if (_currentAnimation.isDone())
        {
            if (dist < 2*CELL_SIZE)
            {
                // Need to create damage variable
                _target->addLife(5);
            }
        }
    }
    else
    {
        _getTarget();
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
        const Vec2& coord = _body.getPosition();
        float x = coord.x;
        float y = coord.y;

        GraphicUtils::initQuad(_vertexArray, sf::Vector2f(288, 311), sf::Vector2f(144, 155), SCALE*0.25);
        sf::VertexArray& vertices(_vertexArray);
        GraphicUtils::transform(vertices, sf::Vector2f(x, y), _angle);

        _currentAnimation.applyOnQuad(vertices, _time);

        GameRender::addQuad(_currentAnimation.getTexture(), vertices, RenderLayer::RENDER);
        GameRender::addShadowCaster(getCoord(), CELL_SIZE);
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
}

void Zombie::hit(WorldEntity* entity, GameWorld* gameWorld)
{
    switch(entity->getType())
    {
        case(EntityTypes::BULLET) :
        {
            Bullet* bullet = static_cast<Bullet*>(entity);
            const Vec2& pos(bullet->getCoord());
            float bulletAngle = bullet->getAngle();

            getBody().accelerate2D(bullet->getImpactForce());
            addLife(-bullet->getDamage());
            _time = rand()%1000;

            if (GameRender::isVisible(this))
            {
                gameWorld->addEntity(ExplosionProvider::getBase(pos));
                if (bullet->getDistance() < 50)
                {
                    gameWorld->addEntity(ExplosionProvider::getClose(pos, bulletAngle));
                    gameWorld->addEntity(Guts::add(&entity->getBody(), pos, bullet->getV()*40.f));
                }

                if (bullet->getPenetration()>-1)
                {
                    gameWorld->addEntity(ExplosionProvider::getThrough(pos, bulletAngle, true));
                    gameWorld->addEntity(ExplosionProvider::getBigThrough(pos, bulletAngle));
                    gameWorld->addEntity(ExplosionProvider::getHit(pos, bulletAngle, true));
                }
                else
                {
                    gameWorld->addEntity(ExplosionProvider::getHit(pos, bulletAngle+PI, true));
                }
            }

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

void Zombie::_getTarget()
{
    ListPtr<Bot>& bots = Bot::getObjects();
    ListPtr<Hunter>& hunters = Hunter::getObjects();

    WorldEntity* target = nullptr;
    float minDist  = -1;

    for (Shared<Bot>& bot : bots)
    {
        Vec2 v(bot->getCoord(), getCoord());
        float dist = v.getNorm2();

        if (dist < minDist || minDist < 0)
        {
            minDist = dist;
            target = &(*bot);
        }
    }

    for (Shared<Hunter>& hunter : hunters)
    {
        Vec2 v(hunter->getCoord(), getCoord());
        float dist = v.getNorm2();

        if (dist < minDist || minDist < 0)
        {
            minDist = dist;
            target = &(*hunter);
        }
    }

    if (target)
        _target = target;
}




