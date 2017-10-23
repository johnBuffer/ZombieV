#ifndef BOT_HPP_INCLUDED
#define BOT_HPP_INCLUDED

#include "HunterBase.hpp"

class Bot : public HunterBase, public WorldEntityPool<Bot>
{
public:
    Bot();
    Bot(float x, float y);

    void hit(WorldEntity* entity, GameWorld* gameWorld);
    void setTarget(WorldEntity* entity) {m_target = entity;}
    void update(GameWorld& world);

    static void initialize();

private:
    WorldEntity* m_target;
    size_t m_getTargetCount;

    void computeControls(GameWorld& world);
    void getTarget(GameWorld* world);
};

#endif // BOT_HPP_INCLUDED
