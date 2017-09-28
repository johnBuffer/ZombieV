#include "Props/Fire.hpp"
#include "Config.hpp"
#include "GameRender.hpp"

#include "iostream"

size_t Fire::_textureID;

/// This class builds a simple fire representation for weapons

void Fire::init()
{
    Fire::_textureID = GameRender::registerTexture("data/textures/fire.png");
}

Fire::Fire()
{
    _needsPhysics = false;
}

Fire::Fire(Vec2 pos, float angle, float sizeFactor):
    _vertexArray(sf::Quads, 4),
    _isDone(false)
{
    _body.setPosition(pos);
    _angle = angle;
    _needsPhysics = false;
    _sizeFactor = sizeFactor;
}

void Fire::render()
{
    Vec2 pos(getCoord());

    GraphicUtils::initQuad(_vertexArray, sf::Vector2f(40, 50), sf::Vector2f(20, 0), 1.0f*_sizeFactor);
    GraphicUtils::transform(_vertexArray, sf::Vector2f(pos.x, pos.y), _angle);

    int h = rand()%8;
    int tx = (h%4)*64;
    int ty = (h/4)*64;

    _vertexArray[0].texCoords = sf::Vector2f(tx   , ty);
    _vertexArray[1].texCoords = sf::Vector2f(tx+64, ty);
    _vertexArray[2].texCoords = sf::Vector2f(tx+64, ty+64);
    _vertexArray[3].texCoords = sf::Vector2f(tx   , ty+64);

    GameRender::addQuad(_textureID, _vertexArray, RenderLayer::RENDER);
    GameRender::addQuad(_textureID, _vertexArray, RenderLayer::BLOOM);

    _isDone = true;
}

void Fire::update(GameWorld& world)
{

}

