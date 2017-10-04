#ifndef STANTARDENTITY_HPP_INCLUDED
#define STANTARDENTITY_HPP_INCLUDED

#include "System/WorldEntity.hpp"
#include "System/DrawableEntity.hpp"
#include "System/ChainedObject.hpp"

template<class T>
class StandardEntity : public WorldEntity, public DrawableEntity<T>, public ChainedObject<T>
{
public:
    StandardEntity() : WorldEntity(0.0f, 0.0f, 0.0f) {}
    StandardEntity(float x, float y, float angle) : WorldEntity(x, y, angle) {}
};

#endif // STANTARDENTITY_HPP_INCLUDED
