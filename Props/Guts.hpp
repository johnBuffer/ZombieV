#include <WorldEntity.hpp>
#include <DrawableEntity.hpp>
#include <UnitedEngine/U_2DConstraint.h>

class Guts : public WorldEntity, public DrawableEntity<Guts>
{
public:
    Guts(U_2DBody* anchor, Vec2 pos, Vec2 v);

    bool isDone() const;

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();

    static void init();

private:
    Vec2 _initialVelocity;
    U_2DBody* _anchor;

    std::vector<U_2DBody*> _bodies;
    std::vector<U_2DConstraint*> _constraints;

    static size_t _textureID;
};
