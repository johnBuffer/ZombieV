#include "DynamicBlur.h"

int log2(int i)
{
    int targetlevel = 0;
    while (i >>= 1) ++targetlevel;
    return targetlevel;
}

void applyBlur(sf::RenderTexture& texture, sf::Shader& shader)
{
    shader.setParameter("DIRECTION", 1.0);
    texture.draw(sf::Sprite(texture.getTexture()), &shader);
    shader.setParameter("DIRECTION", 0.0);
    texture.draw(sf::Sprite(texture.getTexture()), &shader);
}

DynamicBlur::DynamicBlur() :
    _downSizeFactor(2.0)
{
    _blur.loadFromFile("blur.frag", sf::Shader::Fragment);
}

void DynamicBlur::init(unsigned int textureWidth, unsigned int textureHeight)
{
    _WIDTH  = textureWidth;
    _HEIGHT = textureHeight;
    _blur.setParameter("WIDTH", _WIDTH);
    _blur.setParameter("HEIGHT", _HEIGHT);
    _blurTexture.create(_WIDTH, _HEIGHT);
    _lowBlurTexture.create(_WIDTH, _HEIGHT);
}

const sf::Texture& DynamicBlur::operator()(const sf::Texture& inputTexture)
{
    sf::Sprite downscaleSprite(inputTexture);
    downscaleSprite.setScale(0.5, 0.5);
    _blurTexture.draw(downscaleSprite);

    _blur.setParameter("SCALE", 2);
    for (int i(2); i--;)
        applyBlur(_blurTexture, _blur);

    _blurTexture.display();

    sf::Sprite downscaledSprite1(_blurTexture.getTexture());
    downscaledSprite1.setScale(2/float(_downSizeFactor), 2/float(_downSizeFactor));
    _lowBlurTexture.draw(downscaledSprite1);
    _lowBlurTexture.display();
    _blurTexture.draw(sf::Sprite(_lowBlurTexture.getTexture()));

    sf::Sprite borderSprite(_lowBlurTexture.getTexture());
    borderSprite.setPosition(_WIDTH/_downSizeFactor, 0);
    _blurTexture.draw(borderSprite);
    borderSprite.setPosition(0, _HEIGHT/_downSizeFactor);
    _blurTexture.draw(borderSprite);

    int i = 2*_downSizeFactor;
    while (i >>= 1 > 0.5)
    {
        _blur.setParameter("SCALE", 1/float(i));
        for (int k(log2(i)); k--;)
        {
            applyBlur(_blurTexture, _blur);
        }

        //applyBlur(_blurTexture, _blur);

        if (i-1)
        {
            sf::Sprite upscale(_blurTexture.getTexture());
            upscale.scale(2, 2);
            _lowBlurTexture.draw(upscale);
            _blurTexture.draw(sf::Sprite(_lowBlurTexture.getTexture()));
        }
    }
    _blurTexture.display();

    return _blurTexture.getTexture();
}

void DynamicBlur::blurRenderTexture(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(sf::Sprite(this->operator()(renderTexture.getTexture())));
}
