#include "Blur/DynamicBlur.h"

int log2(int i)
{
    int targetlevel = 0;
    while (i >>= 1) ++targetlevel;
    return targetlevel;
}

DynamicBlur::DynamicBlur() :
    _downSizeFactor(2.0)
{
    _blurH.loadFromFile("data/shaders/blurH.frag", sf::Shader::Fragment);
    _blurV.loadFromFile("data/shaders/blurV.frag", sf::Shader::Fragment);
}

void DynamicBlur::init(unsigned int textureWidth, unsigned int textureHeight)
{
    _WIDTH  = textureWidth;
    _HEIGHT = textureHeight;

    _blurH.setUniform("WIDTH",  (float)  _WIDTH);
    _blurH.setUniform("HEIGHT", (float) _HEIGHT);
    _blurV.setUniform("WIDTH",  (float) _WIDTH);
    _blurV.setUniform("HEIGHT", (float) _HEIGHT);

    _blurTexture.create(_WIDTH, _HEIGHT);
    _lowBlurTexture.create(_WIDTH, _HEIGHT);
}

void DynamicBlur::_applyBlur(sf::RenderTexture& texture)
{
    texture.draw(sf::Sprite(texture.getTexture()), &_blurH);
    texture.draw(sf::Sprite(texture.getTexture()), &_blurV);
}

const sf::Texture& DynamicBlur::operator()(const sf::Texture& inputTexture)
{
    /// Downscale the texture
    sf::Sprite downscaleSprite(inputTexture);
    downscaleSprite.setScale(0.5, 0.5);
    _blurTexture.draw(downscaleSprite);
    _applyBlur(_blurTexture);
    _blurTexture.display();

    sf::Sprite downscaledSprite1(_blurTexture.getTexture());
    downscaledSprite1.setScale(2/_downSizeFactor, 2/_downSizeFactor);
    _lowBlurTexture.draw(downscaledSprite1);
    _lowBlurTexture.display();
    _blurTexture.draw(sf::Sprite(_lowBlurTexture.getTexture()));

    int i = static_cast<int>(_downSizeFactor);
    while (i > 0)
    {
        _applyBlur(_blurTexture);
        if (i-1)
        {
            sf::Sprite upscale(_blurTexture.getTexture());
            upscale.scale(2, 2);
            _lowBlurTexture.draw(upscale);
            _blurTexture.draw(sf::Sprite(_lowBlurTexture.getTexture()));
        }
		i >>= 1;
    }
    _blurTexture.display();

    return _blurTexture.getTexture();
}

void DynamicBlur::blurRenderTexture(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(sf::Sprite(this->operator()(renderTexture.getTexture())));
}
