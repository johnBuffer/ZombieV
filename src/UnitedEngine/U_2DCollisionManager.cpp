#include "UnitedEngine/UnitedEngine.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#include <System/WorldEntity.hpp>
#include <System/GameWorld.hpp>

#include <thread>

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
    m_mapSize = Vec2();

    m_iterationCount = 0;
    m_currentIteration = 0;

    m_gravity = Vec2();
}

U_2DCollisionManager::U_2DCollisionManager(float timeStep, float bodyRadius, Vec2 mapSize, GameWorld* world) :
    _world(world)
{
    m_timeStep = timeStep;
    m_bodySize = bodyRadius*2;
    m_mapSize = mapSize;

    m_iterationCount = 2;
    m_currentIteration = 0;

    m_gravity = Vec2(0, 100);
}

void U_2DCollisionManager::addBody(U_2DBody* body)
{
    if (!body->getRadius())
        body->setRadius(m_bodySize*0.45);

    //m_bodies.push_back(body);
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
        U_2DBody* b = nullptr;
        while (U_2DBody::getNext(b))
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

    int radius = m_bodySize*0.5f;

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
        float delta = -bodyY+radius;
        body->move2D(Vec2(0, delta));
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

        float currentMass   = currentBody->getMass();
        float currentRadius = currentBody->getRadius();
        float currentX = currentBody->getPosition().x;
        float currentY = currentBody->getPosition().y;

        WorldEntity* currentEntity = currentBody->getEntity();

        for (int k(i+1); k<n_bodies; k++)
        {
            U2DBody_ptr collider  = bodies[k];
            float colliderRadius = collider->getRadius();

            float minDist = currentRadius+colliderRadius;
            float vx, vy, dist2;

            vx = currentX-collider->getPosition().x;
            vy = currentY-collider->getPosition().y;

            dist2 = vx*vx+vy*vy;

            if (dist2 && dist2 < minDist*minDist)
            {
                float dist = sqrt(dist2);
                float deltaDist = minDist-dist;
                float responseVelocity = 0.5f*coeff*deltaDist/(dist);

                vx *= responseVelocity;
                vy *= responseVelocity;

                float colliderMass = collider->getMass();
                float totalMassCoeff = 1.0f/(currentMass+colliderMass);
                float massCoef1 = colliderMass*totalMassCoeff;
                float massCoef2 = currentMass*totalMassCoeff;

                currentBody->move2D(Vec2( vx*massCoef1,  vy*massCoef1));
                collider   ->move2D(Vec2(-vx*massCoef2, -vy*massCoef2));


                WorldEntity* colliderEntity = collider->getEntity();

                if (currentEntity)
                    currentEntity->hit(colliderEntity, _world);
                if (colliderEntity)
                    colliderEntity->hit(currentEntity, _world);
            }
        }
    }
}

void U_2DCollisionManager::solveCollisions()
{
    for(auto &elem : m_grid)  solveGridCollisions(elem.second);
    U_2DBody* b = nullptr;
    while (U_2DBody::getNext(b))
    {
        solveBoundCollisions(b);
    }
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
    U_2DBody* b = nullptr;
    while (U_2DBody::getNext(b))
    {
        addBodyToGrid(b);
    }

    c2.restart();
    for (int i(0); i<m_iterationCount; ++i)
    {
        solveCollisions();

        solveConstraints();
        solveConstraints();
    }
    std::cout << "Phys time : " << c2.getElapsedTime().asMilliseconds() << std::endl;

    // friction
    b = nullptr;
    while (U_2DBody::getNext(b))
    {
        Vec2 velocity = b->getVelocity();
        b->accelerate2D(Vec2(-10*velocity.x, -10*velocity.y));
        b->updatePosition(m_timeStep);
    }
}

void U_2DCollisionManager::solveConstraints()
{
    for (auto& ctr : m_constraints)
    {
        ctr.applyConstraint();
    }

    //m_constraints.remove_if([](U_2DConstraint& c) {return c.isBroken();});
}

void U_2DCollisionManager::applyExplosion(Vec2 explosionCoord, float force)
{
    /*for (U2DBody_ptr body : m_bodies)
    {
        Vec2 v(explosionCoord, body->getPosition());

        float dist = v.getNorm();
        float f = (force/m_timeStep)/(dist*dist*dist);
        v.x *= f;
        v.y *= f;

        if (dist > 100)
            body->accelerate2D(v);
    }*/
}

GridCell* U_2DCollisionManager::getBodyAt(Vec2 coord)
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

void U_2DCollisionManager::killBody(BodyID id)
{
    U_2DBody::getObjectAt(id)->remove();
}

void U_2DCollisionManager::killConstraint(U_2DConstraint* c)
{
    m_constraints.remove_if([=](const U_2DConstraint& c1){return &c1==c;});
}

U_2DConstraint* U_2DCollisionManager::addConstraint(U_2DBody* body1, U_2DBody* body2, float length)
{
    m_constraints.push_back(U_2DConstraint(body1, body2, 100.0, length));
    return &m_constraints.back();
}

size_t U_2DCollisionManager::addBody(const Vec2& coord)
{
    U_2DBody* newBody = U_2DBody::add(coord, 1.0f, false);
    newBody->setRadius(m_bodySize*0.5f);

    return newBody->getIndex();
}

U_2DBody* U_2DCollisionManager::getBodyByID(size_t id)
{
    return U_2DBody::getObjectAt(id);
}















