#include "Turret.hpp"
#include "GameRender.hpp"
#include "GameWorld.hpp"
#include "Utils.hpp"

#include <iostream>

size_t Turret::s_textureID;

Turret::Turret()
{

}

Turret::Turret(float x, float y) :
    WorldEntity(x, y, 0.0f),
    m_target(nullptr)
{
    m_currentState = Turret::IDLE;
    m_accuracy     = 0.05f;
    m_cooldown     = 0.1f;
    m_currentCooldown = 0.0f;

    Light l;
    l.color = sf::Color(255, 127, 0);
    l.intensity = 1.0f;
    l.radius  = 0;
    light = GameRender::getLightEngine().addDurableLight(l);
}

void Turret::initPhysics(GameWorld* world)
{
    world->addBody(&_body);
    _body.setStatic(true);
}

void Turret::update(GameWorld& world)
{
    light->radius = 0;
    if (m_target)
    {
        Vec2 targetPos = m_target->getCoord();
        float vx = targetPos.x - _body.getPosition().x;
        float vy = targetPos.y - _body.getPosition().y;
        float dist = sqrt(vx*vx+vy*vy);

        vx/=dist;
        vy/=dist;

        Vec2 direction(cos(_angle), sin(_angle));
        Vec2 directionNormal(-direction.y, direction.x);

        float dot2 = vx*directionNormal.x + vy*directionNormal.y;
        float coeff = 0.05f;

        float absDot = std::abs(dot2);
        coeff *= absDot;

        if (absDot<0.25f)
        {
            if (m_currentCooldown < 0)
            {
                m_currentState = Turret::SHOOTING;
                fire(&world);
            }
        }

        if (dot2 < 0)
        {
            _angle += coeff;
        }
        else
        {
            _angle -= coeff;
        }

        m_target = m_target->isDone()?nullptr:m_target;
    }
    else
    {
        setTarget(getTarget(&world));
        _angle += DT*0.2f;
    }

    m_currentCooldown -= DT;
}

void Turret::fire(GameWorld* world)
{
    m_currentCooldown = m_cooldown;

    float bulletAngle(getRandomAngle(-m_accuracy, m_accuracy));
    Bullet* newBullet = new Bullet(_angle, 1.5*CELL_SIZE, 20, 0);
    newBullet->init(getCoord(), PI+bulletAngle);
    world->addEntity(newBullet);

    Vec2 fireOut   = transformVec(Vec2(-130, -2), _angle, getCoord());
    Vec2 smokeOut  = transformVec(Vec2(-60, -2), _angle, getCoord());

    Vec2 bulletVel(newBullet->getV());
    float v(rand()%25/1000.0f+0.1);
    world->addEntity(new Smoke(smokeOut, bulletVel*v, 0.0125, 100));

    Vec2 firePos(fireOut);
    world->addEntity(new Fire(firePos, _angle-PIS2, 2));

    light->position = smokeOut;
    light->radius   = 350;
    //world->addEntity(new Fire(firePos, entityAngle, 0.5));
    //world->addEntity(new Fire(firePos, _angle+PI, 0.5));
}

WorldEntity* Turret::getTarget(GameWorld* world) const
{
    Zombie* zombie = Zombie::getHead();
    Zombie* target = nullptr;
    float minDist  = -1;

    while (zombie)
    {
        Vec2 v(zombie->getCoord(), getCoord());
        float dist = v.getNorm();

        if ((dist < minDist && !zombie->isMarked()) || minDist < 0)
        {
            minDist = dist;
            target = zombie;
        }

        zombie = static_cast<Zombie*>(zombie->getPrev());
    }

    if (target)
        target->setMarked(true);

    return target;
}




void Turret::render()
{
    float x = _body.getPosition().x;
    float y = _body.getPosition().y;

    sf::VertexArray va(sf::Quads, 4);
    va[0].texCoords = sf::Vector2f(0.0f, 0.0f);
    va[1].texCoords = sf::Vector2f(353.0f, 0.0f);
    va[2].texCoords = sf::Vector2f(353.0f, 103.0f);
    va[3].texCoords = sf::Vector2f(0.0f, 103.0f);

    GraphicUtils::initQuad(va, sf::Vector2f(353, 103), sf::Vector2f(224, 57), SCALE*0.25f);
    GraphicUtils::transform(va, sf::Vector2f(x, y), _angle);

    GameRender::addQuad(s_textureID, va, RenderLayer::RENDER);
    GameRender::addShadowCaster(getCoord(), CELL_SIZE);
}

void Turret::init()
{
    s_textureID = GameRender::registerTexture("data/textures/turret_v1.png");
}

