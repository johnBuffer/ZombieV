#ifndef STANTARDENTITY_HPP_INCLUDED
#define STANTARDENTITY_HPP_INCLUDED

#include "System/WorldEntity.hpp"
#include "System/WorldEntityPool.hpp"

template<class T>
class StandardEntity : public WorldEntity, public WorldEntityPool<T>
{
public:
    StandardEntity();
    StandardEntity(float x, float y, float angle);
};

template<class T>
StandardEntity<T>::StandardEntity() :
    WorldEntity(0.0f, 0.0f, 0.0f)
{
}

template<class T>
StandardEntity<T>::StandardEntity(float x, float y, float angle) :
    WorldEntity(x, y, angle)
{
}

#endif // STANTARDENTITY_HPP_INCLUDED
