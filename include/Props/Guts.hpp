#include <System/StandardEntity.hpp>
#include <UnitedEngine/U_2DConstraint.h>

class Guts : public StandardEntity<Guts>
{
public:
    Guts();
    Guts(const Vec2& pos, const Vec2& v);

    bool isDone() const;

    void initPhysics(GameWorld* world);
    void update(GameWorld& world);
    void render();

    void kill(GameWorld* world);

    static void init();

private:
    Vec2      _initialVelocity;
    bool      _isDone;
    float     _duration;

    std::vector<BodyID> _bodies;
    std::vector<U_2DConstraint*> _constraints;

    static size_t _textureID;
};
