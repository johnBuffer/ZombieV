#include "Pistol.hpp"

Pistol::Pistol()
{
    __fireCooldown = 0.2f;
    __currentFireCooldown = 0.0f;
    __recoil = 0.0f;

    __shootAnimation = Animation(3, 1, 255, 215, 3, 40);
    __shootAnimation.setTexture(__shootTexture);
    __shootAnimation.setCenter(sf::Vector2f(101, 120));

    __moveAnimation = Animation(7, 3, 258, 220, 20, 40);
    __moveAnimation.setTexture(__moveTexture);
    __moveAnimation.setCenter(sf::Vector2f(101, 120));

    __idleAnimation = Animation(4, 5, 253, 216, 20, 30);
    __idleAnimation.setTexture(__idleTexture);
    __idleAnimation.setCenter(sf::Vector2f(95, 120));

    __releasedTrigger = true;
    __fireDist = 1.8;
}

std::list<Bullet> Pistol::fire()
{
    std::list<Bullet> bullets(0);

    if (!__currentFireCooldown && __releasedTrigger)
    {
        __recoil += 0.15;
        __recoil = __recoil>1.0f?1.0f:__recoil;

        __currentFireCooldown = __fireCooldown;
        bullets.push_back(Bullet(0.0, 1.5*CELL_SIZE, 15, 0));
        bullets.back().setOffset(13.0);

        __releasedTrigger = false;
    }

    return bullets;
}

void Pistol::reload()
{

}

void Pistol::update()
{
    __recoil -= 0.016;
    __recoil  = __recoil<0?0:__recoil;

    __currentFireCooldown -= 0.016;
    __currentFireCooldown  = __currentFireCooldown<0?0:__currentFireCooldown;
}

void Pistol::subLoadTextures()
{
    __shootTexture.loadFromFile("data/textures/hunter/hunter_gun_shoot.png");
    __moveTexture .loadFromFile("data/textures/hunter/hunter_gun_move.png");
    __idleTexture .loadFromFile("data/textures/hunter/hunter_gun_idle.png");
}
