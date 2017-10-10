#include "System/Cooldown.hpp"

Cooldown::Cooldown() :
    m_delay(0.0f),
    m_currentTime(0.0f)
{

}

Cooldown::Cooldown(float delay) :
    m_delay(delay),
    m_currentTime(delay)
{

}

bool Cooldown::isReady() const
{
    return m_currentTime<=0.0f;
}

float Cooldown::getDelay() const
{
    return m_delay;
}

void Cooldown::update(float dt)
{
    m_currentTime -= dt;
    if (m_currentTime < 0)
        m_currentTime = 0;
}

void Cooldown::reset()
{
    m_currentTime = m_delay;
}

float Cooldown::getRatio() const
{
    return 1.0f-m_currentTime/m_delay;
}

void Cooldown::setDelay(float delay)
{
    m_delay = delay;
}

