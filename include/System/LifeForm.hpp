#ifndef LIFEFORM_HPP_INCLUDED
#define LIFEFORM_HPP_INCLUDED


#include "Animation.hpp"

class LifeForm
{
public:
    LifeForm();

protected:
    float _speed;
    float _pv;

    Animation _currentAnimation;
};

#endif // LIFEFORM_HPP_INCLUDED
