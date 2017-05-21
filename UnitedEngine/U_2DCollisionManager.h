#ifndef U_2DCOLISION_H_INCLUDED
#define U_2DCOLISION_H_INCLUDED

#include "U_2DBody.h"

#include<map>
#include<vector>
#include<cmath>
#include<list>
#include <array>
#include <memory>

typedef U_2DBody* U2DBody_ptr;

struct GridCell
{
    std::array<U2DBody_ptr, 100> _bodies;

    int _maxIndex;

    GridCell();
    void add(U2DBody_ptr body);
    void reset();
};

class U_2DCollisionManager
{
public:
    U_2DCollisionManager();
    U_2DCollisionManager(double timeStep, double bodyRadius, U_2DCoord mapSize);

    GridCell* getBodyAt(U_2DCoord coord);

    double      getBodyRadius()       const {return m_bodySize/2;};
    int         getBodiesCount()      const {return m_bodies.size();}
    U_2DCoord   getMapSize()          const {return m_mapSize;};

    void setGravity(U_2DCoord gravityVec) {m_gravity = gravityVec;};
    void setPrecision(unsigned int iterationCount) {m_iterationCount = iterationCount;};
    void applyExplosion(U_2DCoord explosionCoord, double force);
    void killBody(U_2DBody* body);
    void addBody(U_2DBody* body);
    void update();

    int a;

private:
    int       m_nCollisionChecked, m_iterationCount;
    double    m_timeStep, m_bodySize;
    U_2DCoord m_mapSize, m_gravity;

    long m_currentIteration;

    int m_newHash;

    std::list<U2DBody_ptr> m_bodies;
    std::map<long, GridCell> m_grid;

    void addBodyToGrid(U2DBody_ptr body);
    long convertPosToHash(int x, int y) const;
    void applyGravity();
    void solveGridCollisions(GridCell& cell);
    void solveCollisions();
    void solveBoundCollisions(U2DBody_ptr body);
};


#endif // U_2DCOLISION_H_INCLUDED
