#ifndef GRAPHICSUTILS_HPP_INCLUDED
#define GRAPHICSUTILS_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "WorldEntity.hpp"
#include "Config.hpp"
#include <cmath>

struct GraphicUtils
{
public:
    static void init();
    static sf::VertexArray createEntityShadow(WorldEntity* entity);

private:
    static sf::VertexArray __shadow;
};

#endif // GRAPHICSUTILS_HPP_INCLUDED
