#ifndef HUD_HPP_INCLUDED
#define HUD_HPP_INCLUDED

#include "System/WorldEntity.hpp"
#include "SFML/Graphics.hpp"
#include "System/GameWorld.hpp"

class HUD
{
public:
    HUD();

    void update();
    void setCurrentWave(size_t wave) {m_currentWave = wave;}

    void render(const GameWorld& world, sf::RenderTarget* target);

private:
    EntityID m_entity;

    sf::Font m_font;
    sf::Text m_text;

    size_t m_lastScore;
    size_t m_currentWave;
    float  m_scale;
};

#endif // HUD_HPP_INCLUDED
