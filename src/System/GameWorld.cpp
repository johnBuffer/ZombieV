#include "System/GameWorld.hpp"

#include <iostream>
#include "System/GraphicsUtils.hpp"
#include "Hunter.hpp"
#include "Zombie.hpp"
#include "Props/Props.hpp"
#include "Turret.hpp"

GameWorld::GameWorld() :
    _dt(0.016f),
    _level(MAP_SIZE, MAP_SIZE),
    _phyManager(_dt, CELL_SIZE, Vec2(MAP_SIZE, MAP_SIZE), this)
{
    _phyManager.setGravity(Vec2(0.0f, 0.0f));
    _phyManager.setPrecision(1);

    Guts::init();
    Turret::init();
    Zombie::initialize();
}

void GameWorld::initializeWeapons()
{
	Bullet::init();
	BulletShell::init();
	AK::init();
	Shotgun::init();
	Pistol::init();
	Smoke::init();
	Fire::init();
	Explosion::init();
}

void GameWorld::initEventHandler(sf::RenderWindow& window)
{
    _eventManager.init(&window);
}

void GameWorld::addEntity(WorldEntity* entity)
{
    _entities.push_back(entity);
    entity->initPhysics(this);
}

void GameWorld::update()
{
    _cleanEntities();

    sf::Clock c;
    _phyManager.update();
    //std::cout << "Phys time " << c.getElapsedTime().asMilliseconds() << std::endl;

    for (WorldEntity* entity : _entities)
    {
        entity->update(*this);
    }
}

void GameWorld::_cleanEntities()
{
    std::list<WorldEntity*>::iterator it;
    for (it=_entities.begin(); it!=_entities.end(); ++it)
    {
        WorldEntity* entity = *it;
        if (entity->isDying())
        {
            it = _entities.erase(it);
            entity->kill(this);
        }
        else if (entity->isDone())
        {
            entity->setDying();
        }
    }
}

GridCell* GameWorld::getBodiesAt(const Vec2& coord)
{
    return _phyManager.getBodyAt(coord);
}

void GameWorld::render()
{
    for (WorldEntity* entity : _entities)
    {
        entity->render();
    }
}

void GameWorld::removeBody(BodyID id)
{
    _phyManager.killBody(id);
}

U_2DConstraint* GameWorld::addConstraint(BodyID body1, BodyID body2, float length)
{
    return _phyManager.addConstraint(body1, body2, length);
}

BodyID GameWorld::addBody()
{
    return _phyManager.addBody(Vec2(0, 0));
}

void GameWorld::removeConstraint(U_2DConstraint* constraint)
{
    _phyManager.killConstraint(constraint);
}

U_2DBody* GameWorld::getBodyByID(BodyID id)
{
    return U_2DCollisionManager::getBodyByID(id);
}

uint32_t GameWorld::registerEntityClass(AccessFunc func)
{
    m_accessFuncs.push_back(func);

    return static_cast<uint32_t>(m_accessFuncs.size());
}

WorldEntity* GameWorld::getEntityByID(EntityID id)
{
    uint32_t classID  = (static_cast<uint32_t>(id>>32))-1;
    uint32_t entityID = static_cast<uint32_t>(id);

    return m_accessFuncs[classID](entityID);
}






