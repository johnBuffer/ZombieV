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
#include "Weapons/Bullet.hpp"
#include "Props/ExplosionProvider.hpp"
#include "Props/Smoke.hpp"
#include "SoundPlayer.hpp"
#include "LightEngine/LightEngine.hpp"
#include "Turret.hpp"
#include "Props/Wall.hpp"


class GameWorld
{
public:
    GameWorld();
    void initEventHandler(sf::RenderWindow& window);

    U_2DConstraint* addConstraint(BodyID body1, BodyID body2, float length=0.0);
    void addEntity(WorldEntity* entity);
    void removeBody(BodyID id);
    void removeConstraint(U_2DConstraint* constraint);
    void update();
    void render();

    const Level& getLevel()  const {return _level;}
    bool  isInLevelBounds(const Vec2& coord) {return _level.isInBounds(coord);}
    const EventManager& getEvents() const {return _eventManager;}

    BodyID    addBody();
    GridCell* getBodiesAt(const Vec2& coord);
    static U_2DBody* getBodyByID(BodyID id);

private:
    float _dt;
    Level _level;

    std::list<WorldEntity*> _entities;

    U_2DCollisionManager _phyManager;
    EventManager         _eventManager;

    void _cleanEntities();
};

#endif // GAMEWORLD_HPP_INCLUDED
