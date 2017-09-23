#include "LightEngine.hpp"
#include "GameRender.hpp"

void drawLight(const Light& light, sf::RenderTexture& texture)
{
    size_t _quality = 12;
    Vec2 position = light.position;

    sf::VertexArray va(sf::TriangleFan, _quality+1);
    va[0].position = sf::Vector2f(position.x, position.y);
    va[0].color    = light.color;

    sf::Color edgeColor = light.color;
    edgeColor.a = 0.0f;

    float start = light.angle-light.width*0.5f;
    float delta = light.width/float(_quality-1)*DEGRAD;

    for (size_t i(0); i<_quality; ++i)
    {
        float a = start+i*delta;
        float x = position.x+light.radius*cos(a);
        float y = position.y+light.radius*sin(a);

        va[i+1].position = sf::Vector2f(x, y);
        va[i+1].color    = edgeColor;
    }

    sf::RenderStates states;
    states.blendMode = sf::BlendAdd;
    GameRender::renderVertexArray(va, texture, states);
}
