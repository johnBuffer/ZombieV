#ifndef BALL_HPP_INCLUDED
#define BALL_HPP_INCLUDED

#include "System/StandardEntity.hpp"
#include "UnitedEngine/U_2DConstraint.h"

class Ball :  public StandardEntity<Ball>
{
public:
    Ball();
    Ball(const Vec2& pos);

    bool isDone() const {return false;}

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();
    void kill(GameWorld* world);

    static void init();

private:
    static size_t _textureID;
    sf::VertexArray _vertexArray;
};

#endif // BALL_HPP_INCLUDED
