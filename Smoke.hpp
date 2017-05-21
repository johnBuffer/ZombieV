#ifndef SMOKE_HPP_INCLUDED
#define SMOKE_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "MassiveDrawableEntity.hpp"

#include <SFML/Graphics.hpp>

class Smoke : public WorldEntity, public MassiveDrawableEntity<Smoke>
{
public:
    Smoke();
    Smoke(U_2DCoord pos, U_2DCoord vel, float angularVel, float size);

    void update(GameWorld& world);
    void render();

    static void init();
    static GraphicEntity subGetGraphicEntity() {return GraphicEntity(__vertexArray, __texture);}
    static void subResetVertexArray()          {__vertexArray.clear();}

    bool isDone() const {return __isDone;}

private:
    float     __ratio;
    float     __size;
    float     __angularVelocity;
    U_2DCoord __velocity;

    bool  __isDone;

    static sf::VertexArray __vertexArray;
    static sf::Texture     __texture;


};

#endif // SMOKE_HPP_INCLUDED
