#ifndef AK_HPP_INCLUDED
#define AK_HPP_INCLUDED

#include "WeaponStatics.hpp"

class AK : public WeaponStatics<AK>
{
public:
    AK();

    Vec2 getFireOutPosition(const WorldEntity* entity) const;
    bool fire(GameWorld* world, WorldEntity* entity);
    void reload();
    void update();

    static void init();

private:
    Vec2 _fireOut;
    Vec2 _bulletOut;
    Vec2 _shellsOut;

    static std::vector<size_t> m_shootSounds;
};

#endif // AK_HPP_INCLUDED
