#include "Weapons/AK.hpp"
#include "System/GameRender.hpp"
#include "System/GameWorld.hpp"
#include "System/Utils.hpp"
#include <cmath>
#include <iostream>
#include <System/SoundPlayer.hpp>

AK::AK()
{
    _magazineSize        = 30;
    _currentAmmo         = 30;
    _totalAmmo           = 90;
    _recoil              = 0.0f;

    m_fireCooldown = Cooldown(0.075f);

    /// Init weapon animations, could be static
    _shootAnimation = Animation(3, 1, 312, 206, 3, 40);
    _shootAnimation.setTextureID(_shootTextureID);
    _shootAnimation.setCenter(sf::Vector2f(96, 120));

    _moveAnimation = Animation(3, 7, 313, 206, 20, 40);
    _moveAnimation.setTextureID(_moveTextureID);
    _moveAnimation.setCenter(sf::Vector2f(96, 120));

    _idleAnimation = Animation(3, 7, 313, 207, 20, 10);
    _idleAnimation.setTextureID(_idleTextureID);
    _idleAnimation.setCenter(sf::Vector2f(96, 120));

    _reloadAnimation = Animation(3, 7, 322, 217, 20, 15);
    _reloadAnimation.setTextureID(_reloadTextureID);
    _reloadAnimation.setCenter(sf::Vector2f(102, 122));

    /// Distance after which fire blast is drawn
    _bulletOut = Vec2(0, 8);
    _fireOut   = Vec2(50, 8);
    _shellsOut = Vec2(15, 6);
}

bool AK::fire(GameWorld* world, WorldEntity* entity)
{
    /// Check if weapon is ready
    if (isReady())
    {
        --_currentAmmo;
        //SoundPlayer::playInstanceOf(_shootSoundID);

        m_fireCooldown.reset();
        float accuracy = (tanh(4.0f*_recoil))*0.05f;
        float bulletAngle(getRandomAngle(-accuracy, accuracy));

        float entityAngle = entity->getAngle();
        Vec2  entityPos   = entity->getCoord();

        Vec2 bulletOut = transformVec(_bulletOut, entityAngle, entityPos);
        Vec2 fireOut   = transformVec(_fireOut  , entityAngle, entityPos);
        Vec2 shellsOut = transformVec(_shellsOut, entityAngle, entityPos);

        Bullet& newBullet = Bullet::add(bulletAngle, 1.5*CELL_SIZE, 20, 3);
        newBullet.init(bulletOut, entityAngle);
        world->addEntity(&newBullet);

        Vec2 bulletVel(newBullet.getV());
        float v(rand()%25/1000.0f+0.1);
        world->addEntity(new Smoke(fireOut, bulletVel*v, 0.0125, 50));

        Vec2 firePos(fireOut);
        world->addEntity(new Fire(firePos, entityAngle-PIS2));
        world->addEntity(new Fire(firePos, entityAngle, 0.5));
        world->addEntity(new Fire(firePos, entityAngle+PI, 0.5));

        Vec2 shellVec(-bulletVel.y+rand()%11-5, bulletVel.x+rand()%11-5);
        Vec2 shellPos(shellsOut);

        BulletShell& bulletShell(BulletShell::add(shellPos, shellVec*0.15, entityAngle-PIS2));
        world->addEntity(&bulletShell);
        world->addEntity(new Smoke(shellPos, shellVec*0.05, 0.05, 15));

        _recoil += 0.2;
        _recoil = _recoil>1.0f?1.0f:_recoil;

        return true;
    }

    return false;
}

void AK::reload()
{
    size_t neededAmmo  = _magazineSize-_currentAmmo;
    int aviableAmmo = std::min(_totalAmmo, neededAmmo);

    _currentAmmo += aviableAmmo;
    _totalAmmo   -= aviableAmmo;
}

void AK::update()
{
    /// Update recoil and cool down
    _recoil -= DT;
    m_fireCooldown.update(DT);

    if (_recoil<0.0) _recoil = 0.0;
}

Vec2 AK::getFireOutPosition(const WorldEntity* entity) const
{
    return transformVec(_fireOut, entity->getAngle(), entity->getCoord());
}

/// Static texture loading
void AK::init()
{
    _shootTextureID  = GameRender::registerTexture("data/textures/hunter/hunter_shoot.png");
    _moveTextureID   = GameRender::registerTexture("data/textures/hunter/hunter_move.png");
    _idleTextureID   = GameRender::registerTexture("data/textures/hunter/hunter_idle.png");
    _reloadTextureID = GameRender::registerTexture("data/textures/hunter/hunter_rifle_reload.png");

    _shootSoundID = SoundPlayer::registerSound("data/fire1.wav");
}
