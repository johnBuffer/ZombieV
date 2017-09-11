#include "Guts.hpp"
#include "../GameWorld.hpp"

#include <iostream>

size_t Guts::_textureID;

Guts::Guts(U_2DBody* anchor, Vec2 pos, Vec2 v) :
    WorldEntity(pos.x, pos.y, 0.0),
    _initialVelocity(v),
    _anchor(anchor)
{
    _type = PROPS;
}

bool Guts::isDone() const
{
    return false;
}

void Guts::initPhysics(GameWorld* world)
{
    float radius = 1.f;
    Vec2 pos(getCoord());
    _bodies.push_back(&_body);
    _body.setRadius(radius);
    _body.setMass(0.005);

    world->addBody(&_body);

    size_t count=rand()%10+5;
    for (size_t i(0); i<count; ++i)
    {
        U_2DBody* newBody = new U_2DBody(pos+Vec2(rand()%5-2, rand()%5-2), 0.005);
        newBody->setEntity(this);
        newBody->setRadius(radius);
        world->addBody(newBody);

        _bodies.back()->accelerate2D(_initialVelocity*float(1.0/float(i+1)));
        _constraints.push_back(world->addConstraint(newBody, _bodies.back(), 2.0f));
        _bodies.push_back(newBody);
    }
}

void Guts::update(GameWorld& world)
{

}

void Guts::render()
{
    for (auto& body : _bodies)
    {
        sf::Vector2f pos(body->getPosition().x, body->getPosition().y);

        sf::VertexArray va(sf::Quads, 4);
        GraphicUtils::initQuad(va, sf::Vector2f(4, 4), sf::Vector2f(2, 2), 1.0f);
        GraphicUtils::move(va, pos);
        va[0].color = sf::Color::White;
        va[1].color = sf::Color::Red;
        va[2].color = sf::Color::White;
        va[3].color = sf::Color::Red;

        va[0].texCoords = sf::Vector2f(0 , 0);
        va[1].texCoords = sf::Vector2f(70, 0);
        va[2].texCoords = sf::Vector2f(70, 76);
        va[3].texCoords = sf::Vector2f(0 , 76);

        GameRender::addQuad(_textureID, va, RENDER);
    }
}

void Guts::init()
{
    _textureID = GameRender::registerTexture("data/textures/guts.png");
}
