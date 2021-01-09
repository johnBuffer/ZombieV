#ifndef U_2DCOLISION_H_INCLUDED
#define U_2DCOLISION_H_INCLUDED

#include "U_2DBody.h"
#include "U_2DConstraint.h"
#include "UnitedEngine/Types.hpp"

#include <unordered_map>
#include <vector>
#include <cmath>
#include <list>
#include <array>
#include <memory>


typedef U_2DBody* U2DBody_ptr;

class GameWorld;

struct GridCell
{
    std::array<U2DBody_ptr, 1000> _bodies;
    std::array<BodyID, 1000>      _bodiesID;

    int _maxIndex;

    GridCell();
    void add(U2DBody_ptr body);
    void reset();
};

class U_2DCollisionManager
{
public:
    U_2DCollisionManager();
    U_2DCollisionManager(float timeStep, float bodyRadius, Vec2 mapSize, GameWorld* world);

    GridCell* getBodyAt(Vec2 coord);

    float  getBodyRadius() const {return m_bodySize*0.5f;}
    Vec2   getMapSize()    const {return m_mapSize;}

    BodyID addBody(const Vec2& pos);
    U_2DConstraint* addConstraint(BodyID body1, BodyID body2, float length);

    void addRigidBody(Vec2 pos);
    void setGravity(Vec2 gravityVec) {m_gravity = gravityVec;}
    void setPrecision(uint32_t iterationCount) {m_iterationCount = iterationCount;}
    void solveConstraints();
    void applyExplosion(Vec2 explosionCoord, float force);
    void killBody(BodyID id);
    void killConstraint(U_2DConstraint* c);
    void addBody(U_2DBody* body);
    void update();

    static U_2DBody* getBodyByID(BodyID id);

private:
    uint32_t   m_nCollisionChecked, m_iterationCount;
    float m_timeStep, m_bodySize;
    Vec2  m_mapSize, m_gravity;

    long m_currentIteration;
    int  m_newHash;

    std::list<U_2DConstraint> m_constraints;
    std::unordered_map<long, GridCell> m_grid;

    void addBodyToGrid(U2DBody_ptr body);
    long convertPosToHash(int x, int y) const;
    void applyGravity();
    void solveGridCollisions(GridCell& cell);
    void solveCollisions();
    void solveBoundCollisions(U2DBody_ptr body);

    GameWorld* _world;
};


#endif // U_2DCOLISION_H_INCLUDED
