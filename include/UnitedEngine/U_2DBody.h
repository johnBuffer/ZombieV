#ifndef U_2DBODY_H_INCLUDED
#define U_2DBODY_H_INCLUDED

#include "System/PoolObject.hpp"
#include "Vec2.h"

class WorldEntity;

class U_2DBody : public PoolObject<U_2DBody>
{
public:
    U_2DBody();
    U_2DBody(Vec2 position, float mass, bool included=false);
    void move2D(const Vec2& vec);
    void move2D(float vx, float vy);
    void moveLastPos2D(const Vec2& vec);
    void accelerate2D(const Vec2& acc);
    void accelerate2D(float ax, float ay);
    void updatePosition(float timestep);
    void setPosition(const Vec2& pos);
    void setPosition(float x, float y);
    void setX(float x);
    void setY(float y);
	void stop();
	void setStatic(bool isStatic);
	void setPressure(Vec2 pressure);
	void addPressure(Vec2 pressure);
	void setEntity(WorldEntity* entity);
	void setMass(float mass);
	void setRadius(float radius);

    const Vec2& getPosition() const;
    Vec2  getVelocity()       const;
    bool  isStatic()          const;
    bool  isIncluded()        const;
    float getMass()           const;
    float getRadius()         const;

    WorldEntity* getEntity();
    float getAngle(U_2DBody* body);

private:
    void init(Vec2 position, float mass, bool included);

    Vec2  m_position, m_lastPosition, m_acceleration, _pressure;
    float m_radius, m_mass;
    bool  m_static, m_included, m_gridChange;

    WorldEntity* _entity;
};

#endif // U_2DBODY_H_INCLUDED
