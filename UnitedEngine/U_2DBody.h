#ifndef U_2DBODY_H_INCLUDED
#define U_2DBODY_H_INCLUDED

#include "U_2DCoord.h"

class WorldEntity;

class U_2DBody
{
public:
    U_2DBody();
    U_2DBody(U_2DCoord position, double mass, bool included=false);
    void move2D(U_2DCoord vec);
    void moveLastPos2D(U_2DCoord vec);
    void accelerate2D(U_2DCoord acc);
    void updatePosition(double timestep);
    void setPosition(U_2DCoord pos);
    void setX(double x);
    void setY(double y);
	void stop();
	void setStatic(bool isStatic);
	void setPressure(U_2DCoord pressure);
	void addPressure(U_2DCoord pressure);
	void setEntity(WorldEntity* entity);

    const U_2DCoord& getPosition() const;
    U_2DCoord getVelocity() const;
    bool isStatic() const;
    bool isIncluded() const;
    double getMass() const;
    U_2DCoord getPressure();
    WorldEntity* getEntity();

private:
    void init(U_2DCoord position, double mass, bool included);
    U_2DCoord m_position, m_lastPosition, m_acceleration, __pressure;
    double m_radius, m_mass;
    bool m_static, m_included, m_gridChange;
    WorldEntity* __entity;
};

#endif // U_2DBODY_H_INCLUDED
