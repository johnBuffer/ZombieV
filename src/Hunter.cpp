#include "Hunter.hpp"
#include "Config.hpp"
#include "GameWorld.hpp"

#include "Utils.hpp"

#include <iostream>

size_t Hunter::_feetTextureID;
Animation Hunter::_feetAnimation = Animation(5, 4, 172, 124, 20, 30);

Hunter::Hunter(float x, float y)
{
    _body.setX(x);
    _body.setY(y);
    _body.stop();

    _speed = 150;

    _state     = HunterState::IDLE;
    _lastState = HunterState::IDLE;

    _time = 0.0f;

    _vertexArray = sf::VertexArray(sf::Quads, 4);

    _weapons.push_back(new AK);
    //_weapons.push_back(new Shotgun);
    //_weapons.push_back(new Pistol);

    _currentWeapon = _weapons.back();
    _weaponRank    = _weapons.size()-1;
    _currentAnimation = _currentWeapon->getMoveAnimation();

    _feetTime = 0.0f;

    _type = EntityTypes::HUNTER;

    Light light;
    light.color = sf::Color(255, 127, 0);
    light.intensity = 1.0f;
    light.radius  = 0;
    _shootLight = GameRender::getLightEngine().addDurableLight(light);

    light.color = sf::Color(220, 200, 150);
    light.intensity = 1.0f;
    light.radius  = 300;
    light.width = 45;
    _flashlight = GameRender::getLightEngine().addDurableLight(light);

    light.color = sf::Color(100, 100, 100);
    light.intensity = 1.0f;
    light.radius  = 50;
    light.width = 360;
    _littleLight = GameRender::getLightEngine().addDurableLight(light);
}

void Hunter::init()
{
    _feetTextureID = GameRender::registerTexture("data/textures/hunter/hunter_walk.png");
    _feetAnimation.setTextureID(_feetTextureID);
    _feetAnimation.setCenter(sf::Vector2f(76, 62));
}

void Hunter::updateControls(const EventManager& em)
{
    float COS45 = 0.707106f;

    float vx = 0.0f;
    float vy = 0.0f;

    float norm = 1.0f;

    if (em.isKeyPressed(sf::Keyboard::Z))
        vy = -1;
    else if (em.isKeyPressed(sf::Keyboard::S))
        vy = 1;
    if (em.isKeyPressed(sf::Keyboard::Q))
        vx = -1;
    else if (em.isKeyPressed(sf::Keyboard::D))
        vx = 1;
    norm = vx && vy ? COS45 : 1.0f;

    if (em.isKeyPressed(sf::Keyboard::A))
    {
        if (_releasedWeaponSwap)
        {
            _weaponRank++;
            _weaponRank%=_weapons.size();
            _currentWeapon = _weapons[_weaponRank];
            _releasedWeaponSwap = false;
            _currentAnimation.setDone();
        }
    }
    else
        _releasedWeaponSwap = true;

    _lastState = _state;
    if (vx || vy)
        _state = HunterState::MOVING;
    else
        _state = HunterState::IDLE;

    _body.stop();
    _body.accelerate2D(Vec2(vx*norm*_speed, vy*norm*_speed));

    sf::Vector2i mousePos = em.getMousePosition();
    _angle = getAngleFromVec(mousePos);

    _clicking = em.isLeftMousePressed();
    if (!_clicking)
        _currentWeapon->releaseTrigger();
}

void Hunter::update(GameWorld& world)
{
    updateControls(world.getEvents());
    _currentWeapon->update();

    Vec2 pos(getCoord());
    _time += DT;

    if (_state == MOVING)
        _feetTime += DT;

    if (_clicking)
    {
        if (_currentWeapon->fire(&world, this))
            _state = SHOOTING;
        else
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
        bool wait = !_lastState==IDLE;
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

void Hunter::render()
{
    sf::Vector2f spriteSize(_currentAnimation.getSpriteSize());
    sf::Vector2f feetSpriteSize(_feetAnimation.getSpriteSize());

    float x = _body.getPosition().x;
    float y = _body.getPosition().y;

    GraphicUtils::initQuad(_vertexArray, spriteSize, _currentAnimation.getSpriteCenter(), SCALE*0.26f);
    GraphicUtils::transform(_vertexArray, sf::Vector2f(x, y), _angle);
    _currentAnimation.applyOnQuad(_vertexArray, _time);
    GameRender::addQuad(_currentAnimation.getTexture(), _vertexArray, RenderLayer::RENDER);
    GameRender::addShadowCaster(getCoord(), CELL_SIZE);

    GraphicUtils::initQuad(_vertexArray, feetSpriteSize, _feetAnimation.getSpriteCenter(), SCALE*0.3f);
    GraphicUtils::transform(_vertexArray, sf::Vector2f(x, y), _angle);
    _feetAnimation.applyOnQuad(_vertexArray, _feetTime);
    GameRender::addQuad(_feetAnimation.getTexture(), _vertexArray, RenderLayer::RENDER);

    GraphicUtils::createEntityShadow(this);
}

void Hunter::_changeAnimation(Animation& anim, bool wait)
{
    if (_currentAnimation.isDone() || !wait)
    {
        _currentAnimation = anim;
        _time = 0.0f;
    }
}

void Hunter::initPhysics(GameWorld* world)
{
    world->addBody(&_body);
}

