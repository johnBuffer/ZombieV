#include "Props/BulletShell.hpp"
#include "System/GameRender.hpp"
#include "System/GameWorld.hpp"
#include "System/Utils.hpp"

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

	if (BulletShell::getObjectsCount() > 250) {
		_ratio = 0.5f;
	}
	else {
		_ratio = getRandFloat(1.0f, 5.0f);
	}

    _isDone = false;
    _type = EntityTypes::PROPS;
}

void BulletShell::kill(GameWorld* world)
{
    world->removeBody(m_bodyID);
    world->removeBody(_b2);
    world->removeConstraint(_constraint);

    this->remove();
}

void BulletShell::update(GameWorld& world)
{
    _ratio -= DT;
    _isDone = _ratio<0.0f;

    m_coord = getBodyCoord();
}

void BulletShell::render()
{
    sf::Vector2f pos(m_coord.x, m_coord.y);
    U_2DBody* b1 = m_thisBody();
    U_2DBody* b2 = GameWorld::getBodyByID(_b2);
    GraphicUtils::initQuad(_vertexArray, sf::Vector2f(75, 351), sf::Vector2f(37, 175), 0.02f);
    GraphicUtils::transform(_vertexArray, pos, b2->getAngle(b1)+PIS2);

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
    U_2DBody* body = m_initBody(world);
    body->setMass(0.01f);
    body->stop();
    body->setRadius(1.5f);
    body->accelerate2D(_velocity*200);

    _b2 = world->addBody();
    U_2DBody* body2 = world->getBodyByID(_b2);
    body2->setEntity(this);
    body2->setPosition(m_coord.x, m_coord.y+2);
    body2->stop();
    body2->setMass(0.01f);
    body2->setRadius(1.5f);

    _constraint = world->addConstraint(_b2, m_bodyID, 3);
}



