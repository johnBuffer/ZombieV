#include "System/Level.hpp"
#include "System/GameRender.hpp"

Level::Level() :
    _dimension(0.0, 0.0)
{
}

Level::Level(float width, float height) :
    _dimension(width, height),
    _vertexArray(sf::Quads, 4)
{
    _groundTextureID = GameRender::registerTexture("data/ground3.jpg", true);

    size_t scaleDown = 4;
    _vertexArray[0].texCoords = sf::Vector2f(0                     , 0);
    _vertexArray[1].texCoords = sf::Vector2f(scaleDown*_dimension.x, 0);
    _vertexArray[2].texCoords = sf::Vector2f(scaleDown*_dimension.x, scaleDown*_dimension.y);
    _vertexArray[3].texCoords = sf::Vector2f(0                     , scaleDown*_dimension.y);

    _vertexArray[0].position = sf::Vector2f(0           , 0);
    _vertexArray[1].position = sf::Vector2f(_dimension.x, 0);
    _vertexArray[2].position = sf::Vector2f(_dimension.x, _dimension.y);
    _vertexArray[3].position = sf::Vector2f(0           , _dimension.y);

    GameRender::initGround(_groundTextureID, _vertexArray);
}

bool Level::isInBounds(const Vec2& coord) const
{
    return coord.x > 0 && coord.x < _dimension.x && coord.y > 0 && coord.y < _dimension.y;
}

void Level::render()
{
    _vertexArray[0].position = sf::Vector2f(0            , 0);
    _vertexArray[1].position = sf::Vector2f(_dimension.x, 0);
    _vertexArray[2].position = sf::Vector2f(_dimension.x, _dimension.y);
    _vertexArray[3].position = sf::Vector2f(0            , _dimension.y);

    GameRender::addQuad(_groundTextureID, _vertexArray, RENDER);
}


