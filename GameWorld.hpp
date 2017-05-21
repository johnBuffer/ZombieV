#ifndef GAMEWORLD_HPP_INCLUDED
#define GAMEWORLD_HPP_INCLUDED

#include <list>

#include "Level.hpp"
#include "UnitedEngine/UnitedEngine.h"
#include "Zombie.hpp"
#include "Hunter.hpp"
#include "EventManager.hpp"
#include "Config.hpp"
#include "GameRender.hpp"
#include "Bullet.hpp"
#include "Explosion.hpp"
#include "Smoke.hpp"

class GameWorld
{
public:
    GameWorld();
    void initEventHandler(sf::RenderWindow& window);

    void addHunter(U_2DCoord position);
    void addZombie(U_2DCoord position);
    void addBullet(Bullet bullet);
    void addSmoke (Smoke smoke);

    void addExplosion(Explosion explosion, bool isTrace=false);
    void update();
    void render();

    const Level&  getLevel()  const {return __level;}
    Hunter& getHunter() {return __hunters.back();}
    Zombie& getZombie() {return __zombies.back();}

    const std::list<Hunter>& getHunters() const {return __hunters;}
    const std::list<Zombie>& getZombies() const {return __zombies;}

    GridCell* getBodiesAt(U_2DCoord coord);

private:
    double __dt;
    Level  __level;

    std::list<Zombie> __zombies;
    std::list<Hunter> __hunters;
    std::list<Bullet> __bullets;
    std::list<Smoke>  __smokes;
    std::list<Explosion> __explosions;

    U_2DCollisionManager __phyManager;
    EventManager         __eventManager;
};

#endif // GAMEWORLD_HPP_INCLUDED
