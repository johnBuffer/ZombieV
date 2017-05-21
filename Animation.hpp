#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();
    Animation(size_t sheetW, size_t sheetH, size_t spriteW, size_t spriteH, size_t frameCount, float speed);
    void setTexture(sf::Texture& texture);
    void setDone() {__done=true;}
    void setCenter(sf::Vector2f center) {__spriteCenter=center;}

    sf::IntRect getTexCoord(double time);
    const sf::Texture& getTexture() const {return *__texture;}
    sf::Vector2i getSpriteSize()    const {return sf::Vector2i(__spriteW, __spriteH);}
    sf::Vector2f getSpriteCenter()  const {return __spriteCenter;}
    bool isDone() const;

private:
    size_t __sheetW;
    size_t __sheetH;
    size_t __spriteW;
    size_t __spriteH;

    size_t __frameCount;
    float  __animationSpeed;
    bool   __done;

    sf::Vector2f __spriteCenter;
    sf::Texture* __texture;

};

#endif // ANIMATION_HPP_INCLUDED
