#include "Props/BulletShell.hpp"
#include "System/GameRender.hpp"
#include "System/GameWorld.hpp"

#include <iostream>

size_t BulletShell::_textureID;

BulletShell::BulletShell() :
    StandardEntity()
{

}

BulletShell::BulletShell(const Vec2& pos, const Vec2& velocity, float angle) :
    StandardEntity(pos.x, pos.y, angle),
    _velocity(velocity),
    _vertexArray(sf::Quads, 4)
{
    _angularVelocity = 0.2f;
    _ratio  = rand()%10+1;
    _isDone = false;

    U_2DBody* body = m_thisBody();
    body.->setMass(0.01);
    body->stop();
    body.setRadius(1.5);

    _type = EntityTypes::PROPS;

    _b2 = new U_2DBody(Vec2(pos.x, pos.y+2), 0.005);
    _b2->setRadius(1.5);
    _b2->stop();
    _b2->setEntity(this);
}

void BulletShell::kill()
{
    this->remove();
}

void BulletShell::update(GameWorld& world)
{
    _ratio -= DT;
    _isDone = _ratio<0.0f;

    if (_isDone)
    {
        world.removeBody(&_body);
        world.removeBody(_b2);
        world.removeConstraint(_constraint);

        delete _b2;
    }
}

void BulletShell::render()
{
    sf::Vector2f pos(getCoord().x, getCoord().y);

    GraphicUtils::initQuad(_vertexArray, sf::Vector2f(75, 351), sf::Vector2f(37, 175), 0.02f);
    GraphicUtils::transform(_vertexArray, pos, _b2->getAngle(&_body)+PIS2);

    _vertexArray[0].texCoords = sf::Vector2f(0 , 0);
    _vertexArray[1].texCoords = sf::Vector2f(75, 0);
    _vertexArray[2].texCoords = sf::Vector2f(75, 350);
    _vertexArray[3].texCoords = sf::Vector2f(0 , 350);

    if (!_isDone)
    {
        GameRender::addQuad(_textureID, _vertexArray, RenderLayer::RENDER);
    }
    else
    {
        GameRender::addQuad(_textureID, _vertexArray, RenderLayer::GROUND);
    }
}

void BulletShell::init()
{
    _textureID = GameRender::registerTexture("data/textures/shell.png");
}

void BulletShell::initPhysics(GameWorld* world)
{
    world->addBody(&_body);
    world->addBody(_b2);
    _constraint = world->addConstraint(_b2, &_body);
    _body.accelerate2D(_velocity*200);
}
