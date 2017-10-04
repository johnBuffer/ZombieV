#ifndef BOT_HPP_INCLUDED
#define BOT_HPP_INCLUDED

#include "Hunter.hpp"

class Bot : public Hunter
{
public:
    Bot();
    Bot(float x, float y);

    void setTarget(WorldEntity* entity) {m_target = entity;}
    void update(GameWorld& world);

private:
    WorldEntity* m_target;

    void computeControls(GameWorld& world);
    void getTarget(GameWorld* world);
};

#endif // BOT_HPP_INCLUDED
