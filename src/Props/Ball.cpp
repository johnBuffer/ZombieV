#include "Props/Ball.hpp"

size_t Ball::_textureID=0;

Ball::Ball() :
    StandardEntity(),
    _vertexArray(sf::Quads, 4)
{

}

Ball::Ball(const Vec2& pos) :
    StandardEntity(pos.x, pos.y, 0),
    _vertexArray(sf::Quads, 4)
{

}

void Ball::initPhysics(GameWorld* world)
{
    U_2DBody* body = m_initBody(world);
    body->setRadius(BODY_RADIUS);
    body->setMass(1);
    body->stop();
}

void Ball::update(GameWorld& world)
{

}

void Ball::render()
{
    Vec2 pos = getBodyCoord();
    GraphicUtils::initQuad(_vertexArray, sf::Vector2f(640, 639), sf::Vector2f(320, 320), 0.03f);
    GraphicUtils::transform(_vertexArray, sf::Vector2f(pos.x, pos.y), 0);

    _vertexArray[0].texCoords = sf::Vector2f(0  , 0);
    _vertexArray[1].texCoords = sf::Vector2f(640, 0);
    _vertexArray[2].texCoords = sf::Vector2f(640, 639);
    _vertexArray[3].texCoords = sf::Vector2f(0  , 639);

    GameRender::addQuad(_textureID, _vertexArray, RenderLayer::RENDER);
}

void Ball::kill(GameWorld* world)
{
    world->removeBody(m_bodyID);
}

void Ball::init()
{
    _textureID = GameRender::registerTexture("data/textures/round.png");
}
