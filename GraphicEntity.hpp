#ifndef GRAPHICENTITY_HPP_INCLUDED
#define GRAPHICENTITY_HPP_INCLUDED

#include <SFML/Graphics.hpp>

enum DrawTarget
{
    RENDER,
    BLOOM,
    GROUND
};

struct GraphicEntity : public sf::Transformable
{
    GraphicEntity (const sf::VertexArray& va, DrawTarget target=RENDER):
        _vertexArray(&va),
        _texture(nullptr),
        _target(target)
    {

    }

    GraphicEntity (const sf::VertexArray& va, const sf::Texture& tex, DrawTarget target=RENDER):
        _vertexArray(&va),
        _texture(&tex),
        _target(target)
    {

    }

    const sf::VertexArray* _vertexArray;
    const sf::Texture*     _texture;
    const DrawTarget       _target;
};

#endif // GRAPHICENTITY_HPP_INCLUDED
