#ifndef U_2DCONSTRAINT_H_INCLUDED
#define U_2DCONSTRAINT_H_INCLUDED

#include "U_2DBody.h"
#include "UnitedEngine/Types.hpp"
#include <cmath>

class U_2DConstraint
{
public:
    U_2DConstraint(U_2DBody* b1, U_2DBody* b2);
    U_2DConstraint(U_2DBody* b1, U_2DBody* b2, float maxElongationFactor, float length=0);
    void applyConstraint();

    float getAngle();

    void setOnlyTension(bool tension) {m_onlyTension = tension;}

    bool isBroken() const {return m_broken;};

private:
    BodyID m_idBody1, m_idBody2;
    float  m_length, m_maxElongationFactor;
    bool   m_broken;
    bool   m_onlyTension;

};

#endif // U_2DCONSTRAINT_H_INCLUDED
