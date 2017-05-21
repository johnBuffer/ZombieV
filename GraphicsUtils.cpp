#include "GraphicsUtils.hpp"

sf::VertexArray GraphicUtils::__shadow;

void GraphicUtils::init()
{
    __shadow = sf::VertexArray(sf::TriangleFan, 15);
    float x = 0.0f;
    float y = 0.0f;
    __shadow[0].position = sf::Vector2f(x, y);
    __shadow[0].color    = sf::Color(10, 10, 10, 255);
    for (int i(1); i<15; ++i)
    {
        float a = i*2*PI/13.0f;
        __shadow[i].position = sf::Vector2f(x+1.5*CELL_SIZE*cos(a), y+1.5*CELL_SIZE*sin(a));
        __shadow[i].color    = sf::Color(10, 10, 10, 0);
    }
}

sf::VertexArray GraphicUtils::createEntityShadow(WorldEntity* entity)
{
    sf::VertexArray newShadow = __shadow;
    U_2DCoord pos = entity->getCoord();
    for (int i(0); i<15; ++i)
    {
        sf::Vector2f coord = __shadow[i].position;
        newShadow[i].position = sf::Vector2f(coord.x+pos.x, coord.y+pos.y);
    }

    return newShadow;
}
