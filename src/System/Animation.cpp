#include "System/Animation.hpp"
#include <iostream>

Animation::Animation()
{
    _done = true;
}

Animation::Animation(uint32_t sheetW, uint32_t sheetH, uint32_t spriteW, uint32_t spriteH, uint32_t frameCount, float speed):
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

void Animation::setTextureID(uint64_t textureID)
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
	uint32_t currentSprite = uint32_t(_animationSpeed*(time-_startTime));
    if (currentSprite >= _frameCount)
        _done = true;

    currentSprite %= _frameCount;

    const int32_t textureX = currentSprite%_sheetW;
    const int32_t textureY = currentSprite/_sheetW;

    return sf::IntRect(_spriteW*textureX, _spriteH*textureY,
                       _spriteW         , _spriteH);
}

void Animation::applyOnQuad(sf::VertexArray& quad, float time)
{
	uint32_t currentSprite = uint32_t(_animationSpeed*(time-_startTime));
    if (currentSprite >= _frameCount)
        _done = true;

    currentSprite %= _frameCount;

    float textureX = static_cast<float>((currentSprite%_sheetW)*_spriteW);
    float textureY = static_cast<float>((currentSprite/_sheetW)*_spriteH);

    quad[0].texCoords = sf::Vector2f(textureX          , textureY);
    quad[1].texCoords = sf::Vector2f(textureX+_spriteW, textureY);
    quad[2].texCoords = sf::Vector2f(textureX+_spriteW, textureY+_spriteH);
    quad[3].texCoords = sf::Vector2f(textureX          , textureY+_spriteH);
}

bool Animation::isDone() const
{
    return _done;
}
