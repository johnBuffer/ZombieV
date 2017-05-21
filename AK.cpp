#include "AK.hpp"

AK::AK()
{
    __recoil              = 0.0f;
    __fireCooldown        = 0.08f;
    __currentFireCooldown = 0.0f;

    /// Init weapon animations, could be static
    __shootAnimation = Animation(3, 1, 312, 206, 3, 40);
    __shootAnimation.setTexture(__shootTexture);
    __shootAnimation.setCenter(sf::Vector2f(96, 120));

    __moveAnimation = Animation(3, 7, 313, 206, 20, 40);
    __moveAnimation.setTexture(__moveTexture);
    __moveAnimation.setCenter(sf::Vector2f(96, 120));


    __idleAnimation = Animation(3, 7, 313, 207, 20, 30);
    __idleAnimation.setTexture(__idleTexture);
    __idleAnimation.setCenter(sf::Vector2f(96, 120));

    /// Distance after which fire blast is drawn
    __fireDist = 2.2;
}

std::list<Bullet> AK::fire()
{
    /// Offset from the center of the player
    float offset = 8.0;

    /// Bullets container
    std::list<Bullet> bullets(0);

    /// Check if weapon is ready
    if (!__currentFireCooldown)
    {
        __recoil += 0.15;
        __recoil = __recoil>1.0f?1.0f:__recoil;

        __currentFireCooldown = __fireCooldown;
        bullets.push_back(Bullet(0.0, 1.5*CELL_SIZE, 20, 3));
        bullets.back().setOffset(offset);
    }

    return bullets;
}

void AK::reload()
{

}

void AK::update()
{
    /// Update recoil and cool down
    __recoil -= DT;
    __currentFireCooldown -= DT;

    if (__recoil<0.0)
        __recoil = 0.0;

    if (__currentFireCooldown<0.0)
        __currentFireCooldown = 0.0;
}

/// Static texture loading
void AK::subLoadTextures()
{
    __shootTexture.loadFromFile("data/textures/hunter/hunter_shoot.png");
    __moveTexture .loadFromFile("data/textures/hunter/hunter_move.png");
    __idleTexture .loadFromFile("data/textures/hunter/hunter_idle.png");
}
