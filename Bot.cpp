#include "Bot.hpp"
#include "System/GameWorld.hpp"

Bot::Bot() :
    Hunter(0, 0)
{

}

Bot::Bot(float x, float y) :
    Hunter(x, y),
    m_target(nullptr)
{

}

void Bot::update(GameWorld& world)
{
    computeControls(world);
    _currentWeapon->update();

    Vec2 pos(getCoord());
    _time += DT;

    if (_state == SHOOTING)
    {
        if (!_currentWeapon->fire(&world, this))
        {
            if (_currentWeapon->isMagEmpty())
            {
                _changeAnimation(_currentWeapon->getReloadAnimation(), false);
                _state = RELOADING;
            }
        }
    }

    if (_state == RELOADING && _currentAnimation.isDone())
    {
        _currentWeapon->reload();
        _state = IDLE;
    }

    _shootLight->radius = 0;
    if (_state == SHOOTING)
    {
        bool wait = _lastState==SHOOTING;
        _changeAnimation(_currentWeapon->getShootAnimation(), wait);
        _shootLight->radius = 350;
    }
    else if (_state == MOVING)
    {
        bool wait = !(_lastState==IDLE);
        _changeAnimation(_currentWeapon->getMoveAnimation(), wait);
    }
    else
    {
        _changeAnimation(_currentWeapon->getIdleAnimation());
    }

    _shootLight->position = _currentWeapon->getFireOutPosition(this);
    _flashlight->position = _shootLight->position;
    _littleLight->position = _shootLight->position;
    _flashlight->angle = getAngle()+PI;
}

void Bot::computeControls(GameWorld& world)
{
    if (m_target)
    {
        Vec2 vTarget(m_target->getCoord(), getCoord());
        Vec2 direction(cos(_angle), sin(_angle));
        Vec2 directionNormal(-direction.y, direction.x);

        float dist = vTarget.getNorm();
        float vx = vTarget.x/dist;
        float vy = vTarget.y/dist;

        float dot2 = vx*directionNormal.x + vy*directionNormal.y;
        float coeff = 0.1f;

        float absDot = std::abs(dot2);
        coeff *= absDot;

        if (absDot<0.25f)
        {
            /*if (dist > 300)
            {
                _changeState(HunterState::MOVING);
                _feetTime += DT;

                _body.stop();
                _body.accelerate2D(Vec2(-vx*_speed, -vy*_speed));
            }
            else */

            if (dist < 300)
            {
                _changeState(SHOOTING);
                if (dist < 100)
                {
                    _changeState(HunterState::MOVING);
                    _feetTime += DT;

                    _body.stop();
                    _body.accelerate2D(Vec2(vx*_speed, vy*_speed));
                }
            }
            else if (dist < 500)
            {
                float speedFactor = 0.25f;
                _body.stop();
                _body.accelerate2D(Vec2(-vx*_speed*speedFactor, -vy*_speed*speedFactor));
                _feetTime += DT*speedFactor;
            }
            else
            {
                getTarget(&world);
            }
        }
        else
        {
            _changeState(IDLE);
        }

        _angle += dot2>0?-coeff:coeff;

        if (m_target->isDying())
            m_target = nullptr;
    }
    else
    {
        _changeState(IDLE);
        getTarget(&world);
    }
}

void Bot::getTarget(GameWorld* world)
{
    ListPtr<Zombie> zombies = Zombie::getObjects();

    Zombie* target = nullptr;
    float minDist  = -1;

    for (Shared<Zombie>& zombie : zombies)
    {
        Vec2 v(zombie->getCoord(), getCoord());
        float dist = v.getNorm();

        if ((dist < minDist && !zombie->isMarked()) || minDist < 0)
        {
            minDist = dist;
            target = &(*zombie);
        }
    }

    if (target)
        m_target = target;
}