#ifndef ZOMBIE_HPP_INCLUDED
#define ZOMBIE_HPP_INCLUDED

#include "LifeForm.hpp"
#include "GraphicEntity.hpp"
#include "MassiveDrawableEntity.hpp"
#include <memory>

class Zombie : public LifeForm, public MassiveDrawableEntity<Zombie>
{
public:
    Zombie(double x, double y);

    void setTarget(WorldEntity* target);
    void update(GameWorld& world);
    void render();

    static void loadTexture();

    static void subResetVertexArray()          {__vertexArray.clear();}
    static GraphicEntity subGetGraphicEntity() {return GraphicEntity(__vertexArray, __moveAnimation.getTexture());}

private:
    double __pv;
    double __speed;
    double __dmg;
    sf::Color    __color;
    WorldEntity* __target;

    static sf::Texture __moveTexture;
    static Animation   __moveAnimation;
};

#endif // ZOMBIE_HPP_INCLUDED
