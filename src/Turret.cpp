#include "Turret.hpp"
#include "System/GameRender.hpp"
#include "System/GameWorld.hpp"
#include "System/Utils.hpp"
#include "Props/Props.hpp"
#include "Weapons/Bullet.hpp"
#include "Zombie.hpp"

#include <iostream>

size_t Turret::s_textureID;
size_t Turret::s_shootSoundID;

Turret::Turret()
{

}

Turret::Turret(float x, float y) :
    StandardEntity(x, y, 0.0f),
    m_fireCooldown(0.1f),
    m_target(ENTITY_NULL),
    m_autoAim(0.05f)
{
    m_currentState = Turret::IDLE;
    m_accuracy     = 0.05f;

    Light l;
    l.color = sf::Color(255, 127, 0);
    l.intensity = 1.0f;
    l.radius  = 0;
    light = GameRender::getLightEngine().addDurableLight(l);
}

void Turret::initPhysics(GameWorld* world)
{
    U_2DBody* body = m_initBody(world);
    body->setStatic(true);

    m_barrelBody = world->addBody();
    U_2DBody* barrelBody = world->getBodyByID(m_barrelBody);
    barrelBody->setStatic(true);
    barrelBody->setEntity(this);
    barrelBody->setRadius(15);
}

void Turret::update(GameWorld& world)
{
    light->radius = 0;
    if (m_target)
    {
        WorldEntity* target = world.getEntityByID(m_target);

        m_autoAim.setAimingEntity(getID());
        m_autoAim.setTarget(m_target);
        m_autoAim.update(world, DT);

        if (m_autoAim.getDotDist()<0.25f)
        {
            if (m_fireCooldown.isReady())
            {
                m_currentState = Turret::SHOOTING;
                fire(&world);
            }
        }
        else
        {
            m_currentState = Turret::IDLE;
        }

        _angle += m_autoAim.getDelta();


        if (target->isDying())
            m_target = ENTITY_NULL;
    }
    else
    {
        m_currentState = Turret::IDLE;
        setTarget(getTarget(&world));
        _angle += DT*0.2f;
    }

    Vec2 smokeOut = transformVec(Vec2(-45, -2), _angle, getCoord());
    GameWorld::getBodyByID(m_barrelBody)->setPosition(smokeOut);

    m_fireCooldown.update(DT);
}

void Turret::fire(GameWorld* world)
{
    m_fireCooldown.reset();

    float bulletAngle(getRandomAngle(-m_accuracy, m_accuracy));
    Bullet* newBullet(Bullet::add(bulletAngle, 1.5f*CELL_SIZE, 20.0f, 0));
    newBullet->init(getCoord(), PI+_angle);
    newBullet->setImpactForce(2.0f);
    world->addEntity(newBullet);

    Vec2 fireOut   = transformVec(Vec2(-130, -2), _angle, getCoord());
    Vec2 smokeOut  = transformVec(Vec2(-60, -2), _angle, getCoord());

    Vec2 bulletVel(newBullet->getV());
    float v(rand()%25/1000.0f+0.1f);
    world->addEntity(Smoke::add(smokeOut, bulletVel*v, 0.0125f, 100.0f));

    Vec2 firePos(fireOut);
    world->addEntity(Fire::add(firePos, _angle-PIS2, 1.5f));

    light->position = smokeOut;
    light->radius   = 350;

}

EntityID Turret::getTarget(GameWorld* world) const
{
    Zombie* target = nullptr;
    Zombie* zombie = nullptr;

    float minDist  = -1;

    while (Zombie::getNext(zombie))
    {
        Vec2 v(zombie->getCoord(), getCoord());
        float dist = v.getNorm2();

        if (((dist < minDist && !zombie->isMarked()) || minDist < 0) && !zombie->isDying())
        {
            minDist = dist;
            target = zombie;
        }
    }

    if (target)
    {
        target->setMarked(true);
        return target->getID();
    }

    return ENTITY_NULL;
}

void Turret::render()
{
    const Vec2& coord = getBodyCoord();
    float x = coord.x;
    float y = coord.y;

    float recoilDist  = 10.0f;
    float recoilRatio = m_fireCooldown.getRatio()*recoilDist;

    const float textureOffset = (m_currentState==Turret::SHOOTING?103.0f:0.0f);

    sf::VertexArray va(sf::Quads, 4);
    va[0].texCoords = sf::Vector2f(recoilRatio, textureOffset);
    va[1].texCoords = sf::Vector2f(recoilRatio+353.0f+recoilDist, textureOffset);
    va[2].texCoords = sf::Vector2f(recoilRatio+353.0f+recoilDist, textureOffset+103.0f);
    va[3].texCoords = sf::Vector2f(recoilRatio+0.0f, textureOffset+103.0f);

    sf::VertexArray vaTripod(sf::Quads, 4);
    vaTripod[0].texCoords = sf::Vector2f(394, 0.0f);
    vaTripod[1].texCoords = sf::Vector2f(786, 0.0f);
    vaTripod[2].texCoords = sf::Vector2f(786, 206.0f);
    vaTripod[3].texCoords = sf::Vector2f(394, 206.0f);

    GraphicUtils::initQuad(va, sf::Vector2f(353, 103), sf::Vector2f(244, 57), SCALE*0.25f);
    GraphicUtils::transform(va, sf::Vector2f(x, y), _angle);

    GraphicUtils::initQuad(vaTripod, sf::Vector2f(373, 206), sf::Vector2f(210, 125), SCALE*0.37f);
    GraphicUtils::transform(vaTripod, sf::Vector2f(x, y), 0.0f);

    GameRender::addQuad(s_textureID, vaTripod, RenderLayer::RENDER);
    GameRender::addQuad(s_textureID, va      , RenderLayer::RENDER);
    GameRender::addShadowCaster(getCoord(), CELL_SIZE);

    GraphicUtils::createEntityShadow(this);
}

void Turret::init()
{
    s_textureID = GameRender::registerTexture("data/textures/turret_v1.png");
    s_shootSoundID = SoundPlayer::registerSound("data/fire1.wav");
}

void Turret::setTarget(EntityID entity)
{
    m_target = entity;
    m_autoAim.setTarget(entity);
}

