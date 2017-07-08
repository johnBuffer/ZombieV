#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();
    Animation(size_t sheetW, size_t sheetH, size_t spriteW, size_t spriteH, size_t frameCount, float speed);
    void setTextureID(size_t textureID);
    void setDone() {_done=true;}
    void setCenter(sf::Vector2f center) {_spriteCenter=center;}

    sf::IntRect  getTexCoord(float time);
    sf::Vector2f getSpriteSize()   const {return sf::Vector2f(_spriteW, _spriteH);}
    sf::Vector2f getSpriteCenter() const {return _spriteCenter;}
    size_t       getTexture()      const {return _textureID;}

    void applyOnQuad(sf::VertexArray& quad, float time);

    bool isDone() const;

private:
    size_t _sheetW;
    size_t _sheetH;
    size_t _spriteW;
    size_t _spriteH;

    size_t _frameCount;
    float  _animationSpeed;
    bool   _done;

    sf::Vector2f _spriteCenter;
    size_t       _textureID;

};

#endif // ANIMATION_HPP_INCLUDED
