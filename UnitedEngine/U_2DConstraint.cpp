#include "U_2DConstraint.h"

U_2DConstraint::U_2DConstraint(U_2DBody* b1, U_2DBody* b2)
{
    m_body1 = b1;
    m_body2 = b2;

    m_length = Vec2(m_body2->getPosition(), m_body1->getPosition()).getNorm();
    m_broken = false;
    _maxElongationFactor = 0;
    _onlyTension = false;
}

U_2DConstraint::U_2DConstraint(U_2DBody* b1, U_2DBody* b2, float maxElongationFactor, float length)
{
    m_body1 = b1;
    m_body2 = b2;

    m_length = length;
    if (length == 0)
        m_length = Vec2(m_body2->getPosition(), m_body1->getPosition()).getNorm();

    m_broken = false;
    _maxElongationFactor = maxElongationFactor;
    _onlyTension = false;
}


void U_2DConstraint::applyConstraint()
{
    float vx = m_body1->getPosition().x-m_body2->getPosition().x;
    float vy = m_body1->getPosition().y-m_body2->getPosition().y;

    float mass1 = m_body1->getMass();
    float mass2 = m_body2->getMass();

    float coeffMass2 = 1;
    float coeffMass1 = 1;

    if (mass1 != 0 && mass2 != 0)
    {
        coeffMass2 = mass1/(mass1+mass2);
        coeffMass1 = mass2/(mass1+mass2);
    }

    float dist = sqrt(vx*vx+vy*vy);

    if (_maxElongationFactor != 0 && dist > m_length*_maxElongationFactor)
    {
        m_broken = true;
    }
    else
    {
        float delta = dist-m_length;
        float constraintForce = (delta)*0.5/dist;

        vx *= constraintForce;
        vy *= constraintForce;

        if ((_onlyTension && delta>0) || !_onlyTension)
        {
            if (delta < 0)
            {
                coeffMass1 = 1;
                coeffMass2 = 1;
            }

            m_body2->move2D(Vec2(coeffMass2*vx, coeffMass2*vy));
            m_body1->move2D(Vec2(-coeffMass1*vx, -coeffMass1*vy));
        }
    }
}

float U_2DConstraint::getAngle()
{
    Vec2 v = m_body1->getPosition()-m_body2->getPosition();

    float dist = v.getNorm();
    float angle = acos(v.x/dist);

    return v.y>0?angle:-angle;
}

