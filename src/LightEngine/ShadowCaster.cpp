#include "LightEngine/ShadowCaster.hpp"

ShadowCaster::ShadowCaster(const Vec2& pos, float r) :
    m_position(pos),
    m_radius(r)
{

}

const Vec2& ShadowCaster::getPosition() const
{
    return m_position;
}

float ShadowCaster::getRadius() const
{
    return m_radius;
}

bool ShadowCaster::drawShadow(const Light& light, sf::VertexArray& va, size_t index) const
{
    Vec2 lightToCaster(light.position, m_position);
    float dist         = lightToCaster.getNorm();
    float shadowLength = light.radius-dist;
    if (dist < m_radius*0.5f)
    {
        return true;
    }
    else if (shadowLength > 0)
    {
        const float shadowScale = 1.0f;
        const float invDist     = 1.0f/dist;
        Vec2 nrmLightToCaster(lightToCaster.x*invDist, lightToCaster.y*invDist);
        Vec2 normal(-shadowScale*m_radius*nrmLightToCaster.y, shadowScale*m_radius*nrmLightToCaster.x);

        const float normalFactor = 0.5f*light.radius*invDist;
        const float midPointX = m_position.x + nrmLightToCaster.x*shadowLength;
        const float midPointY = m_position.y + nrmLightToCaster.y*shadowLength;

        Vec2 midPoint(midPointX                  , midPointY);
        Vec2 pt1(m_position.x+normal.x           , m_position.y+normal.y);
        Vec2 pt2(midPoint.x+normal.x*normalFactor, midPoint.y+normal.y*normalFactor);
        Vec2 pt3(midPoint.x-normal.x*normalFactor, midPoint.y-normal.y*normalFactor);
        Vec2 pt4(m_position.x-normal.x           , m_position.y-normal.y);

        va[4*index+0].position = sf::Vector2f(pt1.x, pt1.y);
        va[4*index+1].position = sf::Vector2f(pt2.x, pt2.y);
        va[4*index+2].position = sf::Vector2f(pt3.x, pt3.y);
        va[4*index+3].position = sf::Vector2f(pt4.x, pt4.y);

        va[4*index+0].color = sf::Color::Black;
        va[4*index+1].color = sf::Color::Black;
        va[4*index+2].color = sf::Color::Black;
        va[4*index+3].color = sf::Color::Black;
    }

    return false;
}
