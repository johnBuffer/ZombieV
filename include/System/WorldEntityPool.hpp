#ifndef WORLDENTITYPOOL_HPP_INCLUDED
#define WORLDENTITYPOOL_HPP_INCLUDED

#include "System/PoolObject.hpp"
#include "System/GameWorld.hpp"

template<class T>
class WorldEntityPool : public PoolObject<T>
{
public:
    static void registerObject(GameWorld* world);
    static void setClassID(size_t id) {T::m_classID = id;}
    static WorldEntity* getWorldEntityAt(size_t index);
};

template<class T>
void WorldEntityPool<T>::registerObject(GameWorld* world)
{
    T::m_classID = world->registerEntityClass(WorldEntityPool<T>::getWorldEntityAt);
    T::initialize();
}

template<class T>
WorldEntity* WorldEntityPool<T>::getWorldEntityAt(size_t index)
{
    WorldEntity* entity = T::getObjectAt(index);
    return entity;
}

#endif // WORLDENTITYPOOL_HPP_INCLUDED
