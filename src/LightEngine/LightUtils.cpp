#include "LightEngine/LightEngine.hpp"
#include "System/GameRender.hpp"

void drawLight(const Light& light, float quality, sf::RenderTexture& texture)
{
    size_t nPoints = 12;
    Vec2 position = light.position;

    sf::VertexArray va(sf::TriangleFan, nPoints+1);
    va[0].position = sf::Vector2f(position.x, position.y);

    const float coef  = light.intensity;
    va[0].color = sf::Color(static_cast<uint8_t>(light.color.r*coef),
		                    static_cast<uint8_t>(light.color.g*coef),
		                    static_cast<uint8_t>(light.color.b*coef)
		                   );

    sf::Color edgeColor = light.color;
    edgeColor.a = 0;

    float radWidth = light.width*DEGRAD;
    float start = light.angle-radWidth*0.5f+PI;
    float delta = radWidth/float(nPoints-1);

    for (size_t i(0); i<nPoints; ++i)
    {
        float a = start+i*delta;
        float x = position.x+light.radius*cos(a);
        float y = position.y+light.radius*sin(a);

        va[i+1].position = sf::Vector2f(x, y);
        va[i+1].color    = edgeColor;
    }

    sf::RenderStates states;
    states.blendMode = sf::BlendAdd;
    states.transform.scale(quality, quality);
    GameRender::renderVertexArray(va, texture, states);
}
