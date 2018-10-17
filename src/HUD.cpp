#include "HUD.hpp"
#include "System/Utils.hpp"

HUD::HUD()
{
    m_font.loadFromFile("data/font.ttf");
    m_text.setFont(m_font);
    m_text.setCharacterSize(40);
    m_text.setFillColor(sf::Color::White);

    m_currentWave = 0;
}

void HUD::update()
{
    m_scale += (1-m_scale)*0.25f;
}

void HUD::render(const GameWorld& world, sf::RenderTarget* target)
{
    sf::Text originalText = m_text;
    /// Score
    float currentScore = world.getScore();
    if (m_lastScore != currentScore)
    {
        ++m_scale;
    }
    m_lastScore = currentScore;
    m_text.setString(intToString(currentScore));
    m_text.setScale(m_scale, m_scale);
    sf::Rect<float> bbox = m_text.getGlobalBounds();
    m_text.setOrigin(bbox.width*0.25f, bbox.height*0.25f);
    m_text.setPosition(40, 40);
    target->draw(m_text);

    /// Wave
    m_text = originalText;
    m_text.setString(intToString(m_currentWave));
    m_text.setPosition(20, 70);
    target->draw(m_text);

}
