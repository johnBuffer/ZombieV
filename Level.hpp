#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "GraphicEntity.hpp"

class Level
{
public:
    Level();
    Level(double width, double height);

    const sf::Texture&  getTexture() const {return __groundTexture;}
    const sf::Vector2f& getDimension() const {return __dimension;}
    GraphicEntity       render();

private:
    sf::Vector2f    __dimension;
    sf::Texture     __groundTexture;
    sf::VertexArray __vertexArray;

};

#endif // LEVEL_HPP_INCLUDED
