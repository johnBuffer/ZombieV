#ifndef DYNAMICBLUR_H_INCLUDED
#define DYNAMICBLUR_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>

class DynamicBlur
{
public:
    DynamicBlur();
    DynamicBlur(unsigned int texureWidth, unsigned int texureHeight);

    void init(unsigned int textureWidth, unsigned int textureHeight);

    const sf::Texture& operator()(const sf::Texture&);
    void blurRenderTexture(sf::RenderTexture& renderTexture);

    void setDownSizeFactor(unsigned int f) {m_downSizeFactor = pow(2, f);}

private:
    float m_downSizeFactor;

    unsigned int _WIDTH;
    unsigned int _HEIGHT;

	sf::RenderTexture m_blur_tex, m_blur_tex_tmp;

    sf::Shader m_blur, m_blurH, m_blurV, m_vertex;

    void _applyBlur(float scale);

};

#endif // DYNAMICBLUR_H_INCLUDED
