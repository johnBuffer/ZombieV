#ifndef DYNAMICBLUR_H_INCLUDED
#define DYNAMICBLUR_H_INCLUDED

#include <SFML/Graphics.hpp>

class DynamicBlur
{
public:
    DynamicBlur();
    DynamicBlur(unsigned int texureWidth, unsigned int texureHeight);

    void init(unsigned int textureWidth, unsigned int textureHeight);

    const sf::Texture& operator()(const sf::Texture&);
    void blurRenderTexture(sf::RenderTexture& renderTexture);

    void setDownSizeFactor(unsigned int f) {_downSizeFactor = pow(2, f);}

private:
    float _downSizeFactor;

    unsigned int _WIDTH;
    unsigned int _HEIGHT;

    sf::RenderTexture _blurTexture;
    sf::RenderTexture _lowBlurTexture;

    sf::Shader _blur;
};

#endif // DYNAMICBLUR_H_INCLUDED
