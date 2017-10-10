#ifndef COOLDOWN_HPP_INCLUDED
#define COOLDOWN_HPP_INCLUDED

class Cooldown
{
public:
    Cooldown();
    Cooldown(float delay);

    bool  isReady() const;
    float getDelay() const;
    float getRatio() const;

    void setDelay(float delay);
    void update(float dt);
    void reset();

private:
    float m_delay;
    float m_currentTime;
};

#endif // COOLDOWN_HPP_INCLUDED
