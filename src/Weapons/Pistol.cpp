#include "Weapons/Pistol.hpp"
#include "System/GameRender.hpp"
#include "System/GameWorld.hpp"
#include "Props/Props.hpp"

Pistol::Pistol()
{
    m_fireCooldown = Cooldown(0.2f);
    _recoil = 0.0f;

    _shootAnimation = Animation(3, 1, 255, 215, 3, 40);
    _shootAnimation.setTextureID(_shootTextureID);
    _shootAnimation.setCenter(sf::Vector2f(101, 120));

    _moveAnimation = Animation(7, 3, 258, 220, 20, 40);
    _moveAnimation.setTextureID(_moveTextureID);
    _moveAnimation.setCenter(sf::Vector2f(101, 120));

    _idleAnimation = Animation(4, 5, 253, 216, 20, 30);
    _idleAnimation.setTextureID(_idleTextureID);
    _idleAnimation.setCenter(sf::Vector2f(95, 120));

    _releasedTrigger = true;
    _fireDist = 1.5;
}

bool Pistol::fire(GameWorld* world, WorldEntity* entity)
{
    if (m_fireCooldown.isReady() && _releasedTrigger)
    {
        _recoil += 0.15f;
        _recoil = std::min(1.0f, _recoil);

        m_fireCooldown.reset();
        _releasedTrigger = false;

        float entityAngle(entity->getAngle());
        Bullet* newBullet = Bullet::add(0.0f, 1.5f*CELL_SIZE, 7.0f, 0);
        newBullet->init(entity->getCoord(), entityAngle);
        world->addEntity(newBullet);

        Vec2 bulletVel(newBullet->getV());
        Vec2 smokePos = newBullet->getCoord()+bulletVel*1.25f;
        world->addEntity(Smoke::add(smokePos, bulletVel*0.01f, 0.05f, 45.0f));

        Vec2 firePos(newBullet->getCoord()+bulletVel*_fireDist);
        world->addEntity(Fire::add(firePos, entityAngle+PIS2));

        Vec2 shellVec(-bulletVel.y+rand()%11-5, bulletVel.x+rand()%11-5);
        Vec2 shellPos(newBullet->getCoord()+bulletVel*0.6f);
        world->addEntity(BulletShell::add(shellPos, shellVec*0.075f, entityAngle+PIS2));

        return true;
    }

    return false;
}

void Pistol::reload()
{

}

void Pistol::update()
{
    _recoil -= DT;
    _recoil  = std::max(0.0f, _recoil);

    m_fireCooldown.update(DT);
}

void Pistol::init()
{
    _shootTextureID = GameRender::registerTexture("data/textures/hunter/hunter_gun_shoot.png");
    _moveTextureID  = GameRender::registerTexture("data/textures/hunter/hunter_gun_move.png");
    _idleTextureID  = GameRender::registerTexture("data/textures/hunter/hunter_gun_idle.png");
}
