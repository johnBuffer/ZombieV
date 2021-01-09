#include "System/GraphicsUtils.hpp"
#include "System/GameRender.hpp"

sf::VertexArray GraphicUtils::_shadow;
size_t GraphicUtils::_shadowTexID;

void GraphicUtils::init()
{
    _shadow = sf::VertexArray(sf::Quads, 4);
    _shadow[0].texCoords = sf::Vector2f(0  , 0);
    _shadow[1].texCoords = sf::Vector2f(100, 0);
    _shadow[2].texCoords = sf::Vector2f(100, 100);
    _shadow[3].texCoords = sf::Vector2f(0  , 100);
    _shadowTexID = GameRender::registerTexture("data/textures/shadow.png");
}

void GraphicUtils::createEntityShadow(WorldEntity* entity)
{
    const Vec2& pos = entity->getCoord();
    float x(pos.x);
    float y(pos.y);

    _shadow[0].position = sf::Vector2f(x-55, y-55);
    _shadow[1].position = sf::Vector2f(x+55, y-55);
    _shadow[2].position = sf::Vector2f(x+55, y+55);
    _shadow[3].position = sf::Vector2f(x-55, y+55);

    GameRender::addQuad(_shadowTexID, _shadow, RenderLayer::RENDER);
}

sf::VertexArray GraphicUtils::createQuad(sf::Vector2f size, sf::Vector2f center)
{
    sf::VertexArray vertices(sf::Quads, 4);

    float offsetX = center.x;
    float offsetY = center.y;

    float hx = size.x*0.5f;
    float hy = size.y*0.5f;

    vertices[0].position = sf::Vector2f(-hx-offsetX, -hy-offsetY);
    vertices[1].position = sf::Vector2f(hx-offsetX , -hy-offsetY);
    vertices[2].position = sf::Vector2f(hx-offsetX , hy-offsetY);
    vertices[3].position = sf::Vector2f(-hx-offsetX, hy-offsetY);

    return vertices;
}

void GraphicUtils::initQuad(sf::VertexArray& vertices, sf::Vector2f size, sf::Vector2f center, float scale)
{
    float offsetX = center.x*scale;
    float offsetY = center.y*scale;

    float hx = size.x*scale;
    float hy = size.y*scale;

    vertices[0].position = sf::Vector2f(-offsetX  , -offsetY);
    vertices[1].position = sf::Vector2f(hx-offsetX, -offsetY);
    vertices[2].position = sf::Vector2f(hx-offsetX, hy-offsetY);
    vertices[3].position = sf::Vector2f(-offsetX  , hy-offsetY);
}

void GraphicUtils::transform(sf::VertexArray& vertices, sf::Vector2f t, float r)
{
    float cosa = cos(r);
    float sina = sin(r);

    size_t nVertices = vertices.getVertexCount();
    for (size_t i(nVertices); i--;)
    {
        sf::Vertex& v = vertices[i];
        float x = v.position.x;
        float y = v.position.y;

        v.position.x = x*cosa-y*sina + t.x;
        v.position.y = x*sina+y*cosa + t.y;
    }
}

void GraphicUtils::move(sf::VertexArray& vertices, const sf::Vector2f& t)
{
    size_t nVertices = vertices.getVertexCount();
    for (size_t i(nVertices); i--;)
    {
        vertices[i].position.x += t.x;
        vertices[i].position.y += t.y;
    }
}

