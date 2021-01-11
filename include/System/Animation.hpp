#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();
    Animation(uint32_t sheetW, uint32_t sheetH, uint32_t spriteW, uint32_t spriteH, uint32_t frameCount, float speed);
    void setTextureID(uint64_t textureID);
    void setDone() {_done=true;}
    void setCenter(sf::Vector2f center) {_spriteCenter=center;}
    void resetTime(float time);

    sf::IntRect  getTexCoord(float time);
    sf::Vector2f getSpriteSize()   const {return sf::Vector2f(static_cast<float>(_spriteW), static_cast<float>(_spriteH));}
    sf::Vector2f getSpriteCenter() const {return _spriteCenter;}
	uint64_t       getTexture()      const {return _textureID;}
	uint64_t       getCurrentIndex(float time) const {return static_cast<size_t>(_animationSpeed*(time-_startTime));}

    void applyOnQuad(sf::VertexArray& quad, float time);

    bool isDone() const;

private:
    uint32_t _sheetW;
    uint32_t _sheetH;
    uint32_t _spriteW;
    uint32_t _spriteH;

    float  _startTime;

	uint32_t _frameCount;
    float  _animationSpeed;
    bool   _done;

    sf::Vector2f _spriteCenter;
	uint64_t     _textureID;

};

#endif // ANIMATION_HPP_INCLUDED
