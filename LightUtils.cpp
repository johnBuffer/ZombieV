#include "LightEngine.hpp"
#include "GameRender.hpp"

void drawLight(const Light& light, sf::RenderTexture& texture)
{
    size_t _quality = 32;
    Vec2 position = light.position->getCoord();

    sf::VertexArray va(sf::TriangleFan, _quality+1);
    va[0].position = sf::Vector2f(position.x, position.y);
    va[0].color    = light.color;

    sf::Color edgeColor = light.color;
    edgeColor.a = 0.0f;

    for (size_t i(1); i<_quality+1; ++i)
    {
        float a = i*2*PI/(_quality-1);
        float x = position.x+light.radius*cos(a);
        float y = position.y+light.radius*sin(a);

        va[i].position = sf::Vector2f(x, y);
        va[i].color    = edgeColor;
    }

    sf::RenderStates states;
    states.blendMode = sf::BlendAdd;
    GameRender::renderVertexArray(va, texture, states);
}
