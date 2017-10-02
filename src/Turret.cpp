#include "Turret.hpp"
#include "System/GameRender.hpp"
#include "System/GameWorld.hpp"
#include "System/Utils.hpp"

#include <iostream>

size_t Turret::s_textureID;
size_t Turret::s_shootSoundID;

Turret::Turret()
{

}

Turret::Turret(float x, float y) :
    WorldEntity(x, y, 0.0f),
    m_target(nullptr)
{
    m_currentState = Turret::IDLE;
    m_accuracy     = 0.05f;
    m_cooldown     = 0.075f;
    m_currentCooldown = 0.0f;

    Light l;
    l.color = sf::Color(255, 127, 0);
    l.intensity = 1.0f;
    l.radius  = 0;
    light = GameRender::getLightEngine().addDurableLight(l);

    _needsPhysics = true;
}

void Turret::initPhysics(GameWorld* world)
{
    world->addBody(&_body);
    _body.setStatic(true);

    m_body = new U_2DBody(Vec2(0, 0), 1);
    m_body->setStatic(true);
    m_body->setEntity(this);
    m_body->setRadius(15);
    world->addBody(m_body);
}

void Turret::update(GameWorld& world)
{
    light->radius = 0;
    if (m_target)
    {
        Vec2 vTarget(m_target->getCoord(), getCoord());
        Vec2 direction(cos(_angle), sin(_angle));
        Vec2 directionNormal(-direction.y, direction.x);

        float dist = vTarget.getNorm();
        float vx = vTarget.x/dist;
        float vy = vTarget.y/dist;

        float dot2 = vx*directionNormal.x + vy*directionNormal.y;
        float coeff = 0.05f;

        float absDot = std::abs(dot2);
        coeff *= absDot;

        if (absDot<0.25f)
        {
            if (m_currentCooldown <= 0)
            {
                m_currentState = Turret::SHOOTING;
                fire(&world);
            }
        }
        else
        {
            m_currentState = Turret::IDLE;
        }

        if (dot2 < 0)
        {
            _angle -= coeff;
        }
        else
        {
            _angle += coeff;
        }

        m_target = m_target->isDone()?nullptr:m_target;
    }
    else
    {
        m_currentState = Turret::IDLE;
        setTarget(getTarget(&world));
        _angle += DT*0.2f;
    }

    Vec2 smokeOut = transformVec(Vec2(-45, -2), _angle, getCoord());
    m_body->setPosition(smokeOut);

    m_currentCooldown -= DT;
    if (m_currentCooldown<0.0f)
        m_currentCooldown = 0.0f;
}

void Turret::fire(GameWorld* world)
{
    m_currentCooldown = m_cooldown;

    float bulletAngle(getRandomAngle(-m_accuracy, m_accuracy));
    Bullet* newBullet = new Bullet(_angle, 1.5*CELL_SIZE, 20, 0);
    newBullet->init(getCoord(), PI+bulletAngle);
    newBullet->setImpactForce(2.0f);
    world->addEntity(newBullet);

    Vec2 fireOut   = transformVec(Vec2(-130, -2), _angle, getCoord());
    Vec2 smokeOut  = transformVec(Vec2(-60, -2), _angle, getCoord());

    Vec2 bulletVel(newBullet->getV());
    float v(rand()%25/1000.0f+0.1);
    world->addEntity(new Smoke(smokeOut, bulletVel*v, 0.0125, 100));

    Vec2 firePos(fireOut);
    world->addEntity(new Fire(firePos, _angle-PIS2, 1.5f));

    light->position = smokeOut;
    light->radius   = 350;

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

        zombie = zombie->prev;
    }

    if (target)
        target->setMarked(true);

    return target;
}




void Turret::render()
{
    float x = _body.getPosition().x;
    float y = _body.getPosition().y;

    float recoilDist  = 10.0f;
    float recoilRatio = (1.0f-m_currentCooldown/m_cooldown)*recoilDist;

    float textureOffset = m_currentState==Turret::SHOOTING?103:0;

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

    GraphicUtils::initQuad(vaTripod, sf::Vector2f(373, 206), sf::Vector2f(210, 125), SCALE*0.37);
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

