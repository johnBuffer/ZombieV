#include "System/GameRender.hpp"
#include "System/Config.hpp"

#include <iostream>

/*float              GameRender::_quality;
float              GameRender::_zoom;
float              GameRender::_ratio;
sf::Vector2u       GameRender::_renderSize;
sf::Vector2f       GameRender::_focus;
sf::Vector2f       GameRender::_baseOffset;
sf::RenderTexture  GameRender::_renderTexture;
sf::RenderTexture  GameRender::_blurTexture;
sf::RenderTexture  GameRender::_groundTexture;
sf::VertexArray    GameRender::_groundQuad;
size_t             GameRender::_groundTextureID;
size_t             GameRender::_drawCalls;

std::vector<sf::Texture>                  GameRender::_textures;
std::vector<std::vector<sf::VertexArray>> GameRender::_vertices;
std::list<ShadowCaster>                   GameRender::_screenSpaceEntities;

DynamicBlur GameRender::_blur;
LightEngine GameRender::_lightEngine;*/

GameRender* GameRender::s_instance = nullptr;
size_t      GameRender::_drawCalls = 0;

GameRender::GameRender()
{

}

GameRender* GameRender::getInstance()
{
    return s_instance;
}

void GameRender::initialize(size_t width, size_t height)
{
    if (!getInstance())
        s_instance = new GameRender;
    GameRender& instance = *getInstance();
    instance._quality = 1.0f;
    instance._zoom    = 1.5f;

    instance._ratio = instance._zoom/instance._quality;
    instance._focus = sf::Vector2f(0.0, 0.0);
    instance._renderSize = sf::Vector2u(width*instance._quality, height*instance._quality);

    float bx = instance._renderSize.x/(instance._quality*instance._zoom)*0.5f;
    float by = instance._renderSize.y/(instance._quality*instance._zoom)*0.5f;
    instance._baseOffset = sf::Vector2f(bx, by);

    instance._renderTexture.create(instance._renderSize.x, instance._renderSize.y);
    instance._blurTexture.create(instance._renderSize.x, instance._renderSize.y);
    instance._groundTexture.create(MAP_SIZE, MAP_SIZE);

    instance._blur.init(instance._renderSize.x*0.5f, instance._renderSize.y*0.5f);
    instance._blur.setDownSizeFactor(2);
    instance._lightEngine.init(instance._renderSize.x, instance._renderSize.y);

    instance._vertices.resize(3);

    GraphicUtils::init();
}

void GameRender::setFocus(const sf::Vector2f& focus)
{
    GameRender& instance = *getInstance();
    instance._focus = focus;
}

void GameRender::clear()
{
    GameRender& instance = *getInstance();
    instance._drawCalls = 0;

    for (std::vector<sf::VertexArray>& v : instance._vertices)
    {
        for (sf::VertexArray& va : v) va.clear();
    }

    instance._renderTexture.clear(sf::Color::Black);
    renderVertexArray(instance._groundQuad, instance._renderTexture, instance._groundTextureID);
    instance._screenSpaceEntities.clear();
    instance._lightEngine.clear();
}

/// Adds a quad to be rendered in the current frame
void GameRender::addQuad(size_t textureID, const sf::VertexArray& quad, RenderLayer layer)
{
    sf::VertexArray* va(&getInstance()->_vertices[layer][textureID]);
    va->append(quad[0]);
    va->append(quad[1]);
    va->append(quad[2]);
    va->append(quad[3]);
}

void GameRender::addShadowCaster(const Vec2& position, float radius)
{
    getInstance()->_screenSpaceEntities.push_back(ShadowCaster(position, radius));
}

/// Loads and adds a new texture in the render engine
size_t GameRender::registerTexture(std::string filename, bool isRepeated)
{
    GameRender& instance = *getInstance();

    instance._textures.push_back(sf::Texture());
	instance._textures.back().setRepeated(isRepeated);
	instance._vertices[RenderLayer::RENDER].push_back(sf::VertexArray(sf::Quads, 0));
	instance._vertices[RenderLayer::GROUND].push_back(sf::VertexArray(sf::Quads, 0));
	instance._vertices[RenderLayer::BLOOM].push_back(sf::VertexArray(sf::Quads, 0));

    if (instance._textures.back().loadFromFile(filename))
    {
        std::cout << "Add new texture : " << filename << " with ID " << instance._vertices[RenderLayer::RENDER].size() << std::endl;
    }
    else
    {
        std::cout << "Error : cannot load'" << filename << "'" << std::endl;
    }

    return instance._textures.size()-1;
}

///
void GameRender::renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target)
{
    sf::RenderStates states;
    getInstance()->_translateToFocus(states.transform);

    target.draw(va, states);
}

void GameRender::renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target, sf::RenderStates states)
{
    getInstance()->_translateToFocus(states.transform);

    target.draw(va, states);
}

void GameRender::renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target, size_t textureID)
{
    GameRender& instance = *getInstance();

    sf::RenderStates states;
    instance._translateToFocus(states.transform);
    states.texture = &instance._textures[textureID];

    target.draw(va, states);
}

/// Draws a vertexArray in the texture
void GameRender::_renderVertices(std::vector<sf::VertexArray>& vertices, sf::RenderTexture& target, sf::RenderStates& states)
{
    GameRender& instance = *getInstance();

    size_t size(vertices.size());
    for (size_t i(0); i<size; ++i)
    {
        if (vertices[i].getVertexCount())
        {
            states.texture = &instance._textures[i];
            target.draw(vertices[i], states);
            ++instance._drawCalls;
        }
    }
}

/// Finalizes the textures
void GameRender::display(sf::RenderTarget* target)
{
    GameRender& instance = *getInstance();

    sf::Transform tf;
    tf.scale(instance._quality, instance._quality);
    _translateToFocus(tf);
    sf::RenderStates states;
    states.transform = tf;

    renderGround();

    instance._renderVertices(instance._vertices[RenderLayer::RENDER], instance._renderTexture, states);

    /// Draw lights
    sf::Sprite lightSprite(instance._lightEngine.render());
    instance._renderTexture.draw(lightSprite, sf::BlendMultiply);
    instance._renderTexture.display();

    sf::Sprite renderSprite(instance._renderTexture.getTexture());
    renderSprite.setScale(instance._ratio, instance._ratio);
    target->draw(renderSprite);
}

/// Determines if something is in the render view
bool GameRender::isVisible(WorldEntity* entity)
{
    return isVisible(entity->getCoord(), 2*CELL_SIZE);
}

bool GameRender::isVisible(const Vec2& position, float radius)
{
    GameRender& instance = *getInstance();

    float screenPosX = position.x-instance._focus.x;
    float screenPosY = position.y-instance._focus.y;

    return (std::abs(screenPosX) < instance._baseOffset.x+radius && std::abs(screenPosY) < instance._baseOffset.y+radius);
}


const sf::Texture& GameRender::getBlur(const sf::Texture& texture)
{
    return getInstance()->_blur(texture);
}

void GameRender::initGround(size_t textureID, sf::VertexArray& quad)
{
    GameRender& instance = *getInstance();

    instance._groundTextureID = textureID;
    instance._groundQuad = quad;
    instance._groundTexture.clear(sf::Color(0.0, 0.0, 0.0, 0.0));
}


/// Draw the ground
void GameRender::renderGround()
{
    GameRender& instance = *getInstance();

    sf::RenderStates states;
    instance._renderVertices(instance._vertices[RenderLayer::GROUND], instance._groundTexture, states);

    instance._groundTexture.display();
    sf::Sprite groundSprite(instance._groundTexture.getTexture());
    groundSprite.setScale(instance._quality, instance._quality);
    float tx = (instance._focus.x-instance._baseOffset.x)*instance._quality;
    float ty = (instance._focus.y-instance._baseOffset.y)*instance._quality;

    groundSprite.setPosition(-tx, -ty);
    instance._renderTexture.draw(groundSprite, states);
}

void GameRender::fadeGround()
{
    GameRender& instance = *getInstance();

    sf::Vector2i size(instance._groundTexture.getSize());
    sf::RectangleShape rectangle(sf::Vector2f(size.x, size.y));
    rectangle.setFillColor(sf::Color(255, 255, 255, 254));
    instance._groundTexture.draw(rectangle, sf::BlendMultiply);
}

void GameRender::_translateToFocus(sf::Transform& transform)
{
    GameRender& instance = *getInstance();
    transform.translate(instance._baseOffset.x-instance._focus.x, instance._baseOffset.y-instance._focus.y);
}

LightEngine& GameRender::getLightEngine()
{
    GameRender& instance = *getInstance();
    return instance._lightEngine;
}

const std::list<ShadowCaster>& GameRender::getScreenSpaceShadowCasters()
{
    return getInstance()->_screenSpaceEntities;
}
