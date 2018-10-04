#include "Props/Ball.hpp"

size_t Ball::_textureID;

Ball::Ball() :
    StandardEntity()
{

}

Ball::Ball(const Vec2& pos) :
    StandardEntity(pos.x, pos.y, 0.0)
{

}

bool Ball::isDone() const
{
    return false;
}

void Ball::initPhysics(GameWorld* world)
{
    m_initBody(world)->setRadius(10);
}

void Ball::update(GameWorld& world)
{

}

void Ball::render()
{
    Vec2 coord = getBodyCoord();
    sf::Vector2f pos(coord.x, coord.y);

    sf::VertexArray va(sf::Quads, 4);
    GraphicUtils::initQuad(va, sf::Vector2f(640, 638), sf::Vector2f(320, 320), 0.03f);
    GraphicUtils::move(va, pos);

    va[0].texCoords = sf::Vector2f(0  , 0);
    va[1].texCoords = sf::Vector2f(640, 0);
    va[2].texCoords = sf::Vector2f(640, 639);
    va[3].texCoords = sf::Vector2f(0  , 639);

    GameRender::addQuad(_textureID, va, RenderLayer::RENDER);
}

void Ball::kill(GameWorld* world)
{
    this->remove();
}

void Ball::init()
{
    _textureID = GameRender::registerTexture("data/textures/round.png");
}
