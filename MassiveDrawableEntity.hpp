#ifndef MASSIVEDRAWABLEENTITY_HPP_INCLUDED
#define MASSIVEDRAWABLEENTITY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "GraphicEntity.hpp"

template <class T>
class MassiveDrawableEntity
{
public:
    void   render();
    static GraphicEntity getGraphicEntity();
    static void          resetVertexArray();

protected:
    static sf::VertexArray __vertexArray;
};

template <class T>
sf::VertexArray MassiveDrawableEntity<T>::__vertexArray(sf::Quads, 0);

template <class T>
void MassiveDrawableEntity<T>::render()
{
    static_cast<T*>(this)->render();
}

template <class T>
GraphicEntity MassiveDrawableEntity<T>::getGraphicEntity()
{
    return T::subGetGraphicEntity();
}

template <class T>
void MassiveDrawableEntity<T>::resetVertexArray()
{
    T::subResetVertexArray();
}

#endif // MASSIVEDRAWABLEENTITY_HPP_INCLUDED
