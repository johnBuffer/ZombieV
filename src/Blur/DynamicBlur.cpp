#include "Blur/DynamicBlur.h"

int log2(int i)
{
    int targetlevel = 0;
    while (i >>= 1) ++targetlevel;
    return targetlevel;
}

DynamicBlur::DynamicBlur() :
    m_downSizeFactor(2.0)
{
    m_blurH.loadFromFile("data/shaders/blurH.frag", sf::Shader::Fragment);
    m_blurV.loadFromFile("data/shaders/blurV.frag", sf::Shader::Fragment);
}

void DynamicBlur::init(unsigned int textureWidth, unsigned int textureHeight)
{
    _WIDTH  = textureWidth;
    _HEIGHT = textureHeight;

    m_blurH.setUniform("WIDTH",  (float) _WIDTH);
    m_blurH.setUniform("HEIGHT", (float) _HEIGHT);
    m_blurV.setUniform("WIDTH",  (float) _WIDTH);
    m_blurV.setUniform("HEIGHT", (float) _HEIGHT);

    m_blur_tex.create(_WIDTH, _HEIGHT);
    m_blur_tex_tmp.create(_WIDTH, _HEIGHT);
}

void DynamicBlur::_applyBlur(float scale)
{
	sf::VertexArray va(sf::Quads, 4);
	
	sf::RenderStates rs;
	//rs.texture = &m_blur_tex.getTexture();
	rs.shader = &m_blurH;

	va[0].position = { 0.0f, 0.0f };
	va[0].texCoords = sf::Vector2f(0.0f, 0.0f);

	va[1].position = sf::Vector2f(_WIDTH*scale, 0.0f);
	va[1].texCoords = sf::Vector2f(_WIDTH, 0.0f);

	va[2].position = { _WIDTH*scale, _HEIGHT*scale };
	va[2].texCoords = sf::Vector2f(_WIDTH, _HEIGHT);
	
	va[3].position = { 0.0f,  _HEIGHT*scale };
	va[3].texCoords = sf::Vector2f(0.0f, _HEIGHT);

	m_blur_tex_tmp.draw(va, rs);
	m_blur_tex_tmp.display();

	// Stage 2
	sf::Sprite down_scale_sprite_2(m_blur_tex_tmp.getTexture());
	//down_scale_sprite_2.setTextureRect({0, 0, int32_t(_WIDTH*scale), int32_t(_HEIGHT*scale)});
	m_blur_tex.draw(down_scale_sprite_2);
}

const sf::Texture& DynamicBlur::operator()(const sf::Texture& inputTexture)
{
	m_blur_tex_tmp.clear();

	sf::Sprite input_sprite(inputTexture);
	m_blur_tex.draw(input_sprite);
	m_blur_tex.display();
	
	_applyBlur(1.0f);
	m_blur_tex.display();

	/*sf::Sprite up_scale_sprite(m_blur_tex.getTexture());
	up_scale_sprite.setScale(2.0f, 2.0f);
	m_blur_tex_tmp.draw(up_scale_sprite);

	m_blur_tex_tmp.display();*/

	return m_blur_tex.getTexture();
}

void DynamicBlur::blurRenderTexture(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(sf::Sprite(this->operator()(renderTexture.getTexture())));
}
