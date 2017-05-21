#ifndef FIRE_HPP_INCLUDED
#define FIRE_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "GraphicEntity.hpp"

class Fire : public WorldEntity
{
public:
    Fire();
    Fire(U_2DCoord pos, float angle);

    void update(GameWorld& world);
    void render();

    GraphicEntity getGraphicEntity();

    static void init();

private:
    static sf::Texture __texture;
    sf::VertexArray    __vertexArray;
};

#endif // FIRE_HPP_INCLUDED
