#include "Animation.hpp"

Animation::Animation()
{
    __done = true;
}

Animation::Animation(size_t sheetW, size_t sheetH, size_t spriteW, size_t spriteH, size_t frameCount, float speed):
    __sheetW(sheetW),
    __sheetH(sheetH),
    __spriteW(spriteW),
    __spriteH(spriteH),
    __frameCount(frameCount),
    __animationSpeed(speed)
{
    __done = false;
}

void Animation::setTexture(sf::Texture& texture)
{
    __texture = &texture;
}

/// Returns the texture area corresponding to the
/// current time
sf::IntRect Animation::getTexCoord(double time)
{
    size_t currentSprite = size_t(__animationSpeed*time);
    if (currentSprite >= __frameCount)
        __done = true;

    currentSprite %= __frameCount;

    float textureX = currentSprite%__sheetW;
    float textureY = currentSprite/__sheetW;

    return sf::IntRect(__spriteW*textureX, __spriteH*textureY,
                       __spriteW         , __spriteH);
}

bool Animation::isDone() const
{
    return __done;
}
