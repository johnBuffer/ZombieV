#include "GameWorld.hpp"

#include <iostream>
#include "GraphicsUtils.hpp"
#include "ExplosionProvider.hpp"
#include "Fire.hpp"

GameWorld::GameWorld() :
    __dt(0.016),
    __level(MAP_SIZE, MAP_SIZE),
    __phyManager(__dt, CELL_SIZE, U_2DCoord(MAP_SIZE, MAP_SIZE))
{
    __phyManager.setGravity(U_2DCoord(0.0f, 0.0f));
    __phyManager.setPrecision(2);

    Bullet::loadTexture();
    Explosion::loadTexture();
    Smoke::init();
    Fire::init();
    AK::loadTextures();
    Shotgun::loadTextures();
    Pistol::loadTextures();

    GameRender::render(__level.render());
}

void GameWorld::initEventHandler(sf::RenderWindow& window)
{
    __eventManager.init(&window);
}

void GameWorld::addHunter(U_2DCoord position)
{
    __hunters.push_back(Hunter(position.x, position.y));
    __hunters.back().getBody().setEntity(&__hunters.back());
    __phyManager.addBody(&__hunters.back().getBody());
}

void GameWorld::addZombie(U_2DCoord position)
{
    __zombies.push_back(Zombie(position.x, position.y));
    __zombies.back().getBody().setEntity(&__zombies.back());
    __phyManager.addBody(&__zombies.back().getBody());
}

void GameWorld::addBullet(Bullet bullet)
{
    __bullets.push_back(bullet);
}

void GameWorld::addExplosion(Explosion explosion, bool isTrace)
{
    if (isTrace)
        explosion.setTrace();
    __explosions.push_back(explosion);
}

void GameWorld::addSmoke(Smoke smoke)
{
    __smokes.push_back(smoke);
}

void GameWorld::update()
{
    __hunters.back().updateControls(__eventManager);

    for (auto& hunter : __hunters)
    {
        hunter.update(*this);
    }

    for (auto& zombie : __zombies)
    {
        zombie.update(*this);
        if (!zombie.isAlive())
        {
            __phyManager.killBody(&zombie.getBody());
            addExplosion(ExplosionProvider::getBig(zombie.getCoord()), true);
            addExplosion(ExplosionProvider::getBigFast(zombie.getCoord()));
            addExplosion(ExplosionProvider::getBase(zombie.getCoord()));
        }
    }
    __zombies.remove_if([=](const Zombie& z){return !z.isAlive();});

    for (auto& bullet : __bullets)
    {
        bullet.update(*this);
    }
    __bullets.remove_if([=](const Bullet& b){return b.isDone();});

    for (Explosion& expl : __explosions)
    {
        expl.update(*this);
    }
    __explosions.remove_if([=](const Explosion& e){return e.isDone();});

    for (Smoke& smoke : __smokes)
    {
        smoke.update(*this);
    }
    __smokes.remove_if([=](const Smoke& s){return s.isDone();});

    __phyManager.update();
}

GridCell* GameWorld::getBodiesAt(U_2DCoord coord)
{
    return __phyManager.getBodyAt(coord);
}


void GameWorld::render()
{
    Bullet::resetVertexArray();
    for (auto& bullet : __bullets)
    {
        bullet.render();
    }
    GameRender::render(Bullet::getGraphicEntity());

    for (auto& hunter : __hunters)
    {
        GameRender::render(GraphicUtils::createEntityShadow(&hunter));
        hunter.render();
        GameRender::render(hunter.getGraphicEntity());
    }

    Zombie::resetVertexArray();
    for (auto& zombie : __zombies)
    {
        if (GameRender::isVisible(&zombie))
        {
            GameRender::render(GraphicUtils::createEntityShadow(&zombie));
            zombie.render();
        }
    }
    GameRender::render(Zombie::getGraphicEntity());

    Explosion::resetVertexArray();
    for (auto& expl : __explosions)
    {
        expl.render();
    }
    GameRender::render(Explosion::getGraphicEntity());
    GameRender::render(Explosion::getGraphicEntityTrace());

    Smoke::resetVertexArray();
    for (auto& smoke : __smokes)
    {
        smoke.render();
    }
    GameRender::render(Smoke::getGraphicEntity());
}
