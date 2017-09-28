#include "Weapons/Shotgun.hpp"
#include <GameRender.hpp>
#include <GameWorld.hpp>

Shotgun::Shotgun()
{
    _fireCooldown = 0.05f;
    _currentFireCooldown = 0.0f;
    _recoil = 0.0f;

    _shootAnimation = Animation(3, 1, 312, 206, 3, 10);
    _shootAnimation.setTextureID(_shootTextureID);
    _shootAnimation.setCenter(sf::Vector2f(96, 120));


    _moveAnimation = Animation(3, 7, 313, 206, 20, 40);
    _moveAnimation.setTextureID(_moveTextureID);
    _moveAnimation.setCenter(sf::Vector2f(96, 120));

    _idleAnimation = Animation(3, 7, 313, 207, 20, 30);
    _idleAnimation.setTextureID(_idleTextureID);
    _idleAnimation.setCenter(sf::Vector2f(96, 120));

    _fireDist = 2.2;
}

bool Shotgun::fire(GameWorld* world, WorldEntity* entity)
{
    if (!_currentFireCooldown)
    {
        _recoil += 0.15;
        _recoil = _recoil>1.0f?1.0f:_recoil;

        _currentFireCooldown = _fireCooldown;

        float entityAngle = entity->getAngle();

        int openAngle = 24;
        for (int i(0); i<12; ++i)
        {
            float angle = rand()%openAngle-openAngle/2;
            float speed = (rand()%50+125)/100.f;
            Bullet* newBullet = new Bullet(angle*DEGRAD, speed*CELL_SIZE, 15, 30);
            newBullet->init(entity->getCoord(), entityAngle);
            world->addEntity(newBullet);

            Vec2 bulletVel(newBullet->getV());
            Vec2 smokePos = newBullet->getCoord()+bulletVel*1.25f;
            float v(rand()%50/1000.0f+0.1);
            world->addEntity(new Smoke(smokePos, bulletVel*v, 0.05, 75));

            Vec2 firePos(newBullet->getCoord()+bulletVel*_fireDist);
            world->addEntity(new Fire(firePos, entityAngle+PIS2));
        }

        return true;
    }

    return false;
}

void Shotgun::reload()
{

}

void Shotgun::update()
{
    _recoil -= 0.016;
    _recoil  = _recoil<0?0:_recoil;

    _currentFireCooldown -= 0.016;
    _currentFireCooldown  = _currentFireCooldown<0?0:_currentFireCooldown;
}

void Shotgun::init()
{
    _shootTextureID = GameRender::registerTexture("data/textures/hunter/hunter_shotgun_shoot.png");
    _moveTextureID  = GameRender::registerTexture("data/textures/hunter/hunter_shotgun_move.png");
    _idleTextureID  = GameRender::registerTexture("data/textures/hunter/hunter_shotgun_idle.png");
}
