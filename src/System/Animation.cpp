#include "System/Animation.hpp"
#include <iostream>

Animation::Animation()
{
    _done = true;
}

Animation::Animation(size_t sheetW, size_t sheetH, size_t spriteW, size_t spriteH, size_t frameCount, float speed):
    _sheetW(sheetW),
    _sheetH(sheetH),
    _spriteW(spriteW),
    _spriteH(spriteH),
    _startTime(0),
    _frameCount(frameCount),
    _animationSpeed(speed)
{
    _done = false;
}

void Animation::setTextureID(size_t textureID)
{
    _textureID = textureID;
}

void Animation::resetTime(float time)
{
    _startTime = time;
}


/// Returns the texture area corresponding to the
/// current time
sf::IntRect Animation::getTexCoord(float time)
{
    size_t currentSprite = size_t(_animationSpeed*(time-_startTime));
    if (currentSprite >= _frameCount)
        _done = true;

    currentSprite %= _frameCount;

    float textureX = currentSprite%_sheetW;
    float textureY = currentSprite/_sheetW;

    return sf::IntRect(_spriteW*textureX, _spriteH*textureY,
                       _spriteW         , _spriteH);
}

void Animation::applyOnQuad(sf::VertexArray& quad, float time)
{
    size_t currentSprite = size_t(_animationSpeed*(time-_startTime));
    if (currentSprite >= _frameCount)
        _done = true;

    currentSprite %= _frameCount;

    float textureX = (currentSprite%_sheetW)*_spriteW;
    float textureY = (currentSprite/_sheetW)*_spriteH;

    quad[0].texCoords = sf::Vector2f(textureX          , textureY);
    quad[1].texCoords = sf::Vector2f(textureX+_spriteW, textureY);
    quad[2].texCoords = sf::Vector2f(textureX+_spriteW, textureY+_spriteH);
    quad[3].texCoords = sf::Vector2f(textureX          , textureY+_spriteH);
}

bool Animation::isDone() const
{
    return _done;
}
