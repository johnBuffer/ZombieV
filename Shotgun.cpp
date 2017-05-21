#include "Shotgun.hpp"

Shotgun::Shotgun()
{
    __fireCooldown = 0.75f;
    __currentFireCooldown = 0.0f;
    __recoil = 0.0f;

    __shootAnimation = Animation(3, 1, 312, 206, 3, 10);
    __shootAnimation.setTexture(__shootTexture);
    __shootAnimation.setCenter(sf::Vector2f(96, 120));


    __moveAnimation = Animation(3, 7, 313, 206, 20, 40);
    __moveAnimation.setTexture(__moveTexture);
    __moveAnimation.setCenter(sf::Vector2f(96, 120));

    __idleAnimation = Animation(3, 7, 313, 207, 20, 30);
    __idleAnimation.setTexture(__idleTexture);
    __idleAnimation.setCenter(sf::Vector2f(96, 120));

    __fireDist = 2.2;
}

std::list<Bullet> Shotgun::fire()
{
    std::list<Bullet> bullets(0);

    if (!__currentFireCooldown)
    {
        __recoil += 0.15;
        __recoil = __recoil>1.0f?1.0f:__recoil;

        __currentFireCooldown = __fireCooldown;
        //bullets.push_back(Bullet(0.0, 1.5*CELL_SIZE, 20, 3));

        int openAngle = 24;
        for (int i(0); i<12; ++i)
        {
            float angle = rand()%openAngle-openAngle/2;
            float speed = (rand()%50+125)/100.f;
            bullets.push_back(Bullet(angle*DEGRAD, speed*CELL_SIZE, 15, 0));
            bullets.back().setOffset(8.0);
        }
    }

    return bullets;
}

void Shotgun::reload()
{

}

void Shotgun::update()
{
    __recoil -= 0.016;
    __recoil  = __recoil<0?0:__recoil;

    __currentFireCooldown -= 0.016;
    __currentFireCooldown  = __currentFireCooldown<0?0:__currentFireCooldown;
}

void Shotgun::subLoadTextures()
{
    __shootTexture.loadFromFile("data/textures/hunter/hunter_shotgun_shoot.png");
    __moveTexture .loadFromFile("data/textures/hunter/hunter_shotgun_move.png");
    __idleTexture .loadFromFile("data/textures/hunter/hunter_shotgun_idle.png");
}
