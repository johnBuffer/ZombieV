#ifndef LIFEFORM_HPP_INCLUDED
#define LIFEFORM_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "Animation.hpp"

class LifeForm : public WorldEntity
{
public:
    LifeForm();

protected:
    float _speed;

    Animation _currentAnimation;
};

#endif // LIFEFORM_HPP_INCLUDED
