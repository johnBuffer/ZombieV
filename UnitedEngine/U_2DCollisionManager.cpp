#include "U_2DCollisionManager.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../WorldEntity.hpp"

GridCell::GridCell()
{
    _maxIndex=0;
}

void GridCell::add(U2DBody_ptr body)
{
    _bodies[_maxIndex] = body;
    ++_maxIndex;
}

void GridCell::reset()
{
    _maxIndex = 0;
}

U_2DCollisionManager::U_2DCollisionManager()
{
    m_timeStep = 1;
    m_bodySize = 1;
    m_mapSize = U_2DCoord();

    m_iterationCount = 0;
    m_currentIteration = 0;

    m_gravity = U_2DCoord();
}

U_2DCollisionManager::U_2DCollisionManager(double timeStep, double bodyRadius, U_2DCoord mapSize)
{
    m_timeStep = timeStep;
    m_bodySize = bodyRadius*2;
    m_mapSize = mapSize;

    m_iterationCount = 2;
    m_currentIteration = 0;

    m_gravity = U_2DCoord(0, 100);
}


void U_2DCollisionManager::addBody(U_2DBody* body)
{
      m_bodies.push_back(body);
}

long U_2DCollisionManager::convertPosToHash(int x, int y) const
{
    return x+y*1000000000L;
}

void U_2DCollisionManager::addBodyToGrid(U2DBody_ptr body)
{
    int bodyX = body->getPosition().x;
    int bodyY = body->getPosition().y;
    int caseSize = m_bodySize;
    int gridX = bodyX/caseSize;
    int gridY = bodyY/caseSize;
    int midGrid = caseSize/2;

    m_grid[convertPosToHash(gridX, gridY)].add(body);
    //std::cout << "ADD22 with " << body->getEntity() << std::endl;
    if (bodyX%caseSize > midGrid)
    {
        m_grid[convertPosToHash(gridX+1, gridY)].add(body);
        if (bodyY%caseSize > midGrid)
        {
            m_grid[convertPosToHash(gridX  , gridY+1)].add(body);
            m_grid[convertPosToHash(gridX+1, gridY+1)].add(body);
        }
        else
        {
            m_grid[convertPosToHash(gridX  , gridY-1)].add(body);
            m_grid[convertPosToHash(gridX+1, gridY-1)].add(body);
        }
    }
    else
    {
        m_grid[convertPosToHash(gridX-1, gridY)].add(body);
        if (bodyY%caseSize > midGrid)
        {
            m_grid[convertPosToHash(gridX  , gridY+1)].add(body);
            m_grid[convertPosToHash(gridX-1, gridY+1)].add(body);
        }
        else
        {
            m_grid[convertPosToHash(gridX  , gridY-1)].add(body);
            m_grid[convertPosToHash(gridX-1, gridY-1)].add(body);
        }
    }
}

void U_2DCollisionManager::applyGravity()
{
    if (m_gravity.x || m_gravity.y)
    {
        for (auto b : m_bodies)
        {
            b->accelerate2D(m_gravity);
        }
    }
}

void U_2DCollisionManager::solveBoundCollisions(U2DBody_ptr body)
{
    int maxX = m_mapSize.x;
    int maxY = m_mapSize.y;
    int bodyX = body->getPosition().x;
    int bodyY = body->getPosition().y;

    int radius = m_bodySize/2;

    if (bodyY+radius+1 > maxY)
    {
        body->setY(maxY-radius);
    }
    if (bodyX+radius+1 > maxX)
    {
        body->setX(maxX-radius);
    }
    if (bodyY-radius-1 < 0)
    {
        double delta = -bodyY+radius;
        body->move2D(U_2DCoord(0, delta));
    }
    if (bodyX-radius < 0)
    {
        body->setX(radius);
    }
}

void U_2DCollisionManager::solveGridCollisions(GridCell& cell)
{
    float coeff = 0.25f;

    int n_bodies = cell._maxIndex;
    U2DBody_ptr* bodies = cell._bodies.data();

    for (int i(0); i<n_bodies; ++i)
    {
        U2DBody_ptr currentBody = bodies[i];

        double currentX = currentBody->getPosition().x;
        double currentY = currentBody->getPosition().y;

        for (int k(i+1); k<n_bodies; k++)
        {
            U2DBody_ptr collider = bodies[k];

            double minDist = m_bodySize;

            double vx, vy, dist2;

            vx = currentX-collider->getPosition().x;
            vy = currentY-collider->getPosition().y;

            dist2 = vx*vx+vy*vy;

            if (dist2 && dist2 < minDist*minDist)
            {
                double dist = sqrt(dist2);
                double deltaDist = minDist-dist;

                double responseVelocity = std::min(0.5*coeff*deltaDist/(dist), m_bodySize*0.5*m_timeStep);

                currentBody->addPressure(U_2DCoord(deltaDist*vx, deltaDist*vy));
                collider   ->addPressure(U_2DCoord(deltaDist*vx, deltaDist*vy));

                vx *= responseVelocity;
                vy *= responseVelocity;

                currentBody->move2D(U_2DCoord( vx,  vy));
                collider   ->move2D(U_2DCoord(-vx, -vy));
            }
        }
    }
}

void U_2DCollisionManager::solveCollisions()
{
    for(auto &elem : m_grid) solveGridCollisions(elem.second);
    for (auto &body : m_bodies) solveBoundCollisions(body);
}

void U_2DCollisionManager::update()
{
    m_currentIteration++;

    m_nCollisionChecked = 0;
    m_newHash = 0;
    applyGravity();

    sf::Clock c1, c2;

    c1.restart();
    for (auto &elem : m_grid) elem.second.reset();
    for (auto &body : m_bodies)
    {
        body->setPressure(U_2DCoord());
        addBodyToGrid(body);
    }
    //std::cout << "parse time " << c2.getElapsedTime().asMilliseconds() << " ";// << "ms for " << int(100*m_newHash/double(m_bodies.size())) << "%" << std::endl;

    c2.restart();
    for (int i(0); i<m_iterationCount; ++i)
    {
        solveCollisions();
    }

    for (auto &body : m_bodies)
    {
        U_2DCoord velocity = body->getVelocity();
        body->accelerate2D(U_2DCoord(-10*velocity.x, -10*velocity.y));
    }

    for (auto& body : m_bodies) body->updatePosition(m_timeStep);

    //std::cout << "phys time " << c2.getElapsedTime().asMilliseconds() << " ms" << std::endl;
}

void U_2DCollisionManager::applyExplosion(U_2DCoord explosionCoord, double force)
{
    for (U2DBody_ptr body : m_bodies)
    {
        U_2DCoord v(explosionCoord, body->getPosition());

        double dist = v.getNorm();
        double f = (force/m_timeStep)/(dist*dist*dist);
        v.x *= f;
        v.y *= f;

        if (dist > 100)
            body->accelerate2D(v);
    }
}

GridCell* U_2DCollisionManager::getBodyAt(U_2DCoord coord)
{
    int gridX = coord.x/m_bodySize;
    int gridY = coord.y/m_bodySize;
    long hash = convertPosToHash(gridX, gridY);

    auto bodies = m_grid.find(hash);
    if (bodies != m_grid.end())
    {
        return &(bodies->second);
    }

    return nullptr;
}

void U_2DCollisionManager::killBody(U_2DBody* body)
{
    m_bodies.remove_if([=](U2DBody_ptr& b){return b == body;});
}

