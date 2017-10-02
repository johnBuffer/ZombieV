#ifndef BULLETSHELL_HPP_INCLUDED
#define BULLETSHELL_HPP_INCLUDED

#include "System/WorldEntity.hpp"
#include "System/DrawableEntity.hpp"
#include "UnitedEngine\U_2DConstraint.h"

class BulletShell : public WorldEntity, public DrawableEntity<BulletShell>
{
public:
    BulletShell(const Vec2& pos, const Vec2& velocity, float angle);

    bool isDone() const {return _isDone;}

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();

    static void init();

private:
    float _ratio;
    float _angularVelocity;
    Vec2  _velocity;
    bool  _isDone;

    U_2DBody*       _b2;
    U_2DConstraint* _constraint;

    static size_t _textureID;
    sf::VertexArray _vertexArray;
};

#endif // BULLETSHELL_HPP_INCLUDED
