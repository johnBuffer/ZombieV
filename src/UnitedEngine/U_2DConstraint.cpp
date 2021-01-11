#include "UnitedEngine/U_2DConstraint.h"
#include "UnitedEngine/U_2DCollisionManager.h"

U_2DConstraint::U_2DConstraint(U_2DBody* b1, U_2DBody* b2)
{
    m_idBody1 = b1->getIndex();
    m_idBody2 = b2->getIndex();

    m_length = Vec2(b2->getPosition(), b1->getPosition()).getNorm();
    m_broken = false;
    m_maxElongationFactor = 0;
    m_onlyTension = false;
}

U_2DConstraint::U_2DConstraint(U_2DBody* b1, U_2DBody* b2, float maxElongationFactor, float length)
{
    m_idBody1 = b1->getIndex();
    m_idBody2 = b2->getIndex();

    m_length = length;
    if (length == 0)
        m_length = Vec2(b2->getPosition(), b1->getPosition()).getNorm();

    m_broken = false;
    m_maxElongationFactor = maxElongationFactor;
    m_onlyTension = false;
}


void U_2DConstraint::applyConstraint()
{
    U_2DBody* b1 = U_2DCollisionManager::getBodyByID(m_idBody1);
    U_2DBody* b2 = U_2DCollisionManager::getBodyByID(m_idBody2);

    float vx = b1->getPosition().x-b2->getPosition().x;
    float vy = b1->getPosition().y-b2->getPosition().y;

    float mass1 = b1->getMass();
    float mass2 = b2->getMass();

    float coeffMass2 = 1;
    float coeffMass1 = 1;

    if (mass1 != 0 && mass2 != 0)
    {
        coeffMass2 = mass1/(mass1+mass2);
        coeffMass1 = mass2/(mass1+mass2);
    }

    float dist = sqrt(vx*vx+vy*vy);

    if (m_maxElongationFactor && dist > m_length*m_maxElongationFactor)
    {
        m_broken = true;
    }
    else
    {
        float delta = dist-m_length;
        float constraintForce = (delta)*0.5f/dist;

        vx *= constraintForce;
        vy *= constraintForce;

        if ((m_onlyTension && delta>0) || !m_onlyTension)
        {
            if (delta < 0)
            {
                coeffMass1 = 1.0f;
                coeffMass2 = 1.0f;
            }

            b2->move2D(Vec2( coeffMass2*vx,  coeffMass2*vy));
            b1->move2D(Vec2(-coeffMass1*vx, -coeffMass1*vy));
        }
    }
}

float U_2DConstraint::getAngle()
{
    U_2DBody* b1 = U_2DCollisionManager::getBodyByID(m_idBody1);
    U_2DBody* b2 = U_2DCollisionManager::getBodyByID(m_idBody2);

    Vec2 v = b1->getPosition()-b2->getPosition();

    float dist = v.getNorm();
    float angle = acos(v.x/dist);

    return v.y>0?angle:-angle;
}

