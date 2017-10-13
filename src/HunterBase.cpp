#include "HunterBase.hpp"
#include "System/Config.hpp"
#include "System/GameWorld.hpp"

#include "System/Utils.hpp"

#include <iostream>

size_t HunterBase::_feetTextureID;
Animation HunterBase::_feetAnimation = Animation(5, 4, 172, 124, 20, 30);

HunterBase::HunterBase() :
    WorldEntity(0, 0, 0)
{
}

HunterBase::HunterBase(float x, float y) :
    WorldEntity(x, y, 0)
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

    /*light.color = sf::Color(220, 200, 150);
    light.intensity = 1.0f;
    light.radius  = 300;
    light.width = 45;
    _flashlight = GameRender::getLightEngine().addDurableLight(light);

    light.color = sf::Color(100, 100, 100);
    light.intensity = 1.0f;
    light.radius  = 50;
    light.width = 360;
    _littleLight = GameRender::getLightEngine().addDurableLight(light);*/
}

void HunterBase::init()
{
    _feetTextureID = GameRender::registerTexture("data/textures/hunter/hunter_walk.png");
    _feetAnimation.setTextureID(_feetTextureID);
    _feetAnimation.setCenter(sf::Vector2f(76, 62));
}

void HunterBase::update(GameWorld& world)
{

}

void HunterBase::render()
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

void HunterBase::_changeState(HunterState state)
{
    if (_state != RELOADING)
    {
        _lastState = _state;
        _state = state;
    }
}

void HunterBase::_changeAnimation(Animation& anim, bool wait)
{
    if (_currentAnimation.isDone() || !wait)
    {
        _currentAnimation = anim;
        _time = 0.0f;
    }
}

void HunterBase::initPhysics(GameWorld* world)
{
    world->addBody(&_body);
}

