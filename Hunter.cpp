#include "Hunter.hpp"
#include "Config.hpp"
#include "GameWorld.hpp"

#include <cmath>
#include <iostream>

float sign(float f)
{
    return f<0.0f?-1.f:1.0f;
}

Hunter::Hunter(double x, double y)
{
    __body.setX(x);
    __body.setY(y);

    __speed = 150;

    __state     = HunterState::IDLE;
    __lastState = HunterState::IDLE;

    __time = 0.0f;

    __vertexArray = sf::VertexArray(sf::Quads, 0);

    __weapons.push_back(new AK);
    __weapons.push_back(new Shotgun);
    __weapons.push_back(new Pistol);

    __currentWeapon = __weapons.back();
    __weaponRank    = __weapons.size()-1;
    __currentAnimation = __currentWeapon->getMoveAnimation();

    __angleTarget = 90.0;
}

void Hunter::updateControls(EventManager& em)
{
    float COS45 = 0.707106f;

    float vx = 0.0f;
    float vy = 0.0f;

    float norm = 1.0f;

    if (em.isKeyPressed(sf::Keyboard::Z))
        vy = -1;
    else if (em.isKeyPressed(sf::Keyboard::S))
        vy = 1;
    if (em.isKeyPressed(sf::Keyboard::Q))
        vx = -1;
    else if (em.isKeyPressed(sf::Keyboard::D))
        vx = 1;
    norm = vx && vy ? COS45 : 1.0f;

    if (em.isKeyPressed(sf::Keyboard::A))
    {
        if (__releasedWeaponSwap)
        {
            __weaponRank++;
            __weaponRank%=__weapons.size();
            __currentWeapon = __weapons[__weaponRank];
            __releasedWeaponSwap = false;
            __currentAnimation.setDone();
        }
    }
    else
        __releasedWeaponSwap = true;

    __lastState = __state;
    if (vx || vy)
        __state = HunterState::MOVING;
    else
        __state = HunterState::IDLE;

    __body.stop();
    __body.accelerate2D(U_2DCoord(vx*norm*__speed, vy*norm*__speed));

    sf::Vector2i mousePos = em.getMousePosition();
    float ax = mousePos.x;
    float ay = mousePos.y;
    float anorm = sqrt(ax*ax+ay*ay);
    __angleTarget = acos(ax/anorm);
    __angleTarget = ay < 0 ? -__angleTarget : __angleTarget;

    __clicking = em.isLeftMousePressed();
    if (!__clicking)
        __currentWeapon->releaseTrigger();
}

void Hunter::update(GameWorld& world)
{
    __time += 0.016;

    U_2DCoord pos(getCoord());

    float delta = __angleTarget-__angle;
    if (__angleTarget*__angle<0 && std::abs(delta) > PI)
        delta += 2*PI*sign(__angle);
    if (std::abs(delta) < 0.1) __angle = __angleTarget;
    __angle += delta/2.0;

    if (__clicking)
    {
        std::list<Bullet> bullets(__currentWeapon->fire());
        if (bullets.size())
        {
            int recoilAngle = __currentWeapon->getRecoil()*5+1;
            __angle += DEGRAD*(rand()%recoilAngle-recoilAngle/2);

            for (Bullet& bullet : bullets)
            {
                bullet.init(pos, __angle);
                world.addBullet(bullet);

                U_2DCoord smokePos = bullet.getCoord()+bullet.getV()*1.45f;

                float v(rand()%50/1000.0f+0.1);

                world.addSmoke(Smoke(smokePos, bullet.getV()*v, 0.05, 75));
            }

            double fireDist = __currentWeapon->getFireDist();
            U_2DCoord firePos(bullets.back().getCoord()+bullets.back().getV()*fireDist);
            __fire = Fire(firePos, __angle+PI/2);

            __state = SHOOTING;
        }
        else
            __state = IDLE;
    }

    if (__state == SHOOTING)
    {
        bool wait = __lastState==SHOOTING;
        __changeAnimation(__currentWeapon->getShootAnimation(), wait);
    }
    else if (__state == MOVING)
    {
        bool wait = !__lastState==IDLE;
        __changeAnimation(__currentWeapon->getMoveAnimation(), wait);
    }
    else
    {
        __changeAnimation(__currentWeapon->getIdleAnimation());
    }

    __currentWeapon->update();
}

void Hunter::render()
{
    __vertexArray.clear();

    sf::Vector2i spriteSize(__currentAnimation.getSpriteSize());

    sf::Vector2f corner1(0           , 0);
    sf::Vector2f corner2(spriteSize.x, 0);
    sf::Vector2f corner3(spriteSize.x, spriteSize.y);
    sf::Vector2f corner4(0           , spriteSize.y);

    sf::IntRect texCoords = __currentAnimation.getTexCoord(__time);
    float textureX = texCoords.left;
    float textureY = texCoords.top;

    sf::Vertex vertex1(corner1, sf::Vector2f(textureX                , textureY));
    sf::Vertex vertex2(corner2, sf::Vector2f(textureX+texCoords.width, textureY));
    sf::Vertex vertex3(corner3, sf::Vector2f(textureX+texCoords.width, textureY+texCoords.height));
    sf::Vertex vertex4(corner4, sf::Vector2f(textureX                , textureY+texCoords.height));

    __vertexArray.append(vertex1);
    __vertexArray.append(vertex2);
    __vertexArray.append(vertex3);
    __vertexArray.append(vertex4);
}

std::list<GraphicEntity>& Hunter::getGraphicEntity()
{
    __gas.clear();

    U_2DCoord position = __body.getPosition();
    GraphicEntity ga(__vertexArray, __currentAnimation.getTexture());
    ga.setOrigin(__currentAnimation.getSpriteCenter());
    ga.setPosition(position.x, position.y);
    ga.setRotation(__angle*RADDEG);
    ga.setScale(0.28f, 0.28f);

    __gas.push_back(ga);

    if (__state == SHOOTING)
        __gas.push_back(__fire.getGraphicEntity());

    return __gas;
}

void Hunter::__changeAnimation(Animation& anim, bool wait)
{
    if (__currentAnimation.isDone() || !wait)
    {
        __currentAnimation = anim;
        __time = 0.0f;
    }
}
