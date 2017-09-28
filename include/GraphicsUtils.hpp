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
    static void createEntityShadow(WorldEntity* entity);
    static sf::VertexArray createQuad(sf::Vector2f size, sf::Vector2f center);
    static void initQuad(sf::VertexArray& vertices, sf::Vector2f size, sf::Vector2f center, float scale=1.0f);
    static void transform(sf::VertexArray& vertices, sf::Vector2f t, float r);
    static void move(sf::VertexArray& vertices, const sf::Vector2f& t);

private:
    static sf::VertexArray _shadow;
    static size_t _shadowTexID;
};

#endif // GRAPHICSUTILS_HPP_INCLUDED
