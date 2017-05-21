#include "Level.hpp"

Level::Level() :
    __dimension(0.0, 0.0)
{
}

Level::Level(double width, double height) :
    __dimension(width, height),
    __vertexArray(sf::Quads, 4)
{
    __groundTexture.loadFromFile("data/ground3.jpg");
    __groundTexture.setRepeated(true);

    __vertexArray[0].texCoords = sf::Vector2f(0            , 0);
    __vertexArray[1].texCoords = sf::Vector2f(4*__dimension.x, 0);
    __vertexArray[2].texCoords = sf::Vector2f(4*__dimension.x, 4*__dimension.y);
    __vertexArray[3].texCoords = sf::Vector2f(0              , 4*__dimension.y);
}

GraphicEntity Level::render()
{
    __vertexArray[0].position = sf::Vector2f(0            , 0);
    __vertexArray[1].position = sf::Vector2f(__dimension.x, 0);
    __vertexArray[2].position = sf::Vector2f(__dimension.x, __dimension.y);
    __vertexArray[3].position = sf::Vector2f(0            , __dimension.y);

    return GraphicEntity(__vertexArray, __groundTexture, GROUND);
}
