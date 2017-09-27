#ifndef SHADOWCASTER_HPP_INCLUDED
#define SHADOWCASTER_HPP_INCLUDED

#include "LightEngine/LightEngine.hpp"

class ShadowCaster
{
public:
    ShadowCaster(const Vec2& pos, float r);

    const Vec2& getPosition() const;
    float       getRadius()   const;

    virtual bool drawShadow(const Light& light, sf::VertexArray& va, size_t index) const;

private:
    Vec2  m_position;
    float m_radius;
};

#endif // SHADOWCASTER_HPP_INCLUDED
