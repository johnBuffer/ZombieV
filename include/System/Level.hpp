#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "UnitedEngine/Vec2.h"

class Level
{
public:
    Level();
    Level(float width, float height);

    bool isInBounds(const Vec2& coord) const;

    size_t              getTexture() const {return _groundTextureID;}
    const sf::Vector2f& getDimension() const {return _dimension;}

    void render();

private:
    sf::Vector2f    _dimension;
    size_t          _groundTextureID;
    sf::VertexArray _vertexArray;

};

#endif // LEVEL_HPP_INCLUDED
