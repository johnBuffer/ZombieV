#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include "SFML/Graphics.hpp"
#include "WorldEntity.hpp"

template <class T>
class DrawableEntity
{
public:
    static void initialize();
};

template <class T>
void DrawableEntity<T>::initialize()
{
    T::init();
}

#endif // ENTITY_HPP_INCLUDED
