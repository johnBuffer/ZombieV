#include "Weapons/Pistol.hpp"
#include "GameRender.hpp"
#include "GameWorld.hpp"

Pistol::Pistol()
{
    _fireCooldown = 0.2f;
    _currentFireCooldown = 0.0f;
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
    if (!_currentFireCooldown && _releasedTrigger)
    {
        _recoil += 0.15;
        _recoil = _recoil>1.0f?1.0f:_recoil;

        _currentFireCooldown = _fireCooldown;
        _releasedTrigger = false;

        float entityAngle(entity->getAngle());
        Bullet* newBullet = new Bullet(0.0, 1.5*CELL_SIZE, 7, 0);
        newBullet->init(entity->getCoord(), entityAngle);
        world->addEntity(newBullet);

        Vec2 bulletVel(newBullet->getV());
        Vec2 smokePos = newBullet->getCoord()+bulletVel*1.25f;
        world->addEntity(new Smoke(smokePos, bulletVel*0.01, 0.05, 45));

        Vec2 firePos(newBullet->getCoord()+bulletVel*_fireDist);
        world->addEntity(new Fire(firePos, entityAngle+PIS2));

        Vec2 shellVec(-bulletVel.y+rand()%11-5, bulletVel.x+rand()%11-5);
        Vec2 shellPos(newBullet->getCoord()+bulletVel*0.6);
        world->addEntity(new BulletShell(shellPos, shellVec*0.075, entityAngle+PIS2));

        return true;
    }

    return false;
}

void Pistol::reload()
{

}

void Pistol::update()
{
    _recoil -= 0.016;
    _recoil  = _recoil<0?0:_recoil;

    _currentFireCooldown -= 0.016;
    _currentFireCooldown  = _currentFireCooldown<0?0:_currentFireCooldown;
}

void Pistol::init()
{
    _shootTextureID = GameRender::registerTexture("data/textures/hunter/hunter_gun_shoot.png");
    _moveTextureID  = GameRender::registerTexture("data/textures/hunter/hunter_gun_move.png");
    _idleTextureID  = GameRender::registerTexture("data/textures/hunter/hunter_gun_idle.png");
}
