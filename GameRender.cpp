#include "GameRender.hpp"
#include "Config.hpp"

#include <iostream>

float              GameRender::_quality;
sf::Vector2u       GameRender::_renderSize;
sf::Vector2f       GameRender::_focus;
sf::RenderTexture  GameRender::_renderTexture;
sf::RenderTexture  GameRender::_blurTexture;
sf::RenderTexture  GameRender::_groundTexture;
size_t             GameRender::_drawCalls;

std::vector<sf::Texture>                  GameRender::_textures;
std::vector<std::vector<sf::VertexArray>> GameRender::_vertices;
std::list<Vec2>                           GameRender::_screenSpaceEntities;

DynamicBlur GameRender::_blur;
LightEngine GameRender::_lightEngine;

void GameRender::initialize(size_t width, size_t height)
{
    _quality = 0.5;
    _focus = sf::Vector2f(0.0, 0.0);
    _renderSize = sf::Vector2u(width*_quality, height*_quality);
    _renderTexture.create(_renderSize.x, _renderSize.y);
    _blurTexture.create(_renderSize.x, _renderSize.y);
    _groundTexture.create(MAP_SIZE, MAP_SIZE);
    _blur.init(_renderSize.x, _renderSize.y);
    _blur.setDownSizeFactor(3);
    _lightEngine.init(_renderSize.x, _renderSize.y);
    _vertices.resize(3);

    GraphicUtils::init();
}

void GameRender::setFocus(const sf::Vector2f& focus)
{
    _focus = focus;
}

void GameRender::clear()
{
    _drawCalls = 0;

    for (std::vector<sf::VertexArray>& v : _vertices)
    {
        for (sf::VertexArray& va : v) va.clear();
    }

    _renderTexture.clear(sf::Color::Black);
    _blurTexture.clear(sf::Color::Black);
    _screenSpaceEntities.clear();
}

/// Adds a quad to be rendered in the current frame
void GameRender::addQuad(size_t textureID, const sf::VertexArray& quad, RenderLayer layer, bool castShadow)
{
    sf::VertexArray* va(&_vertices[layer][textureID]);
    va->append(quad[0]);
    va->append(quad[1]);
    va->append(quad[2]);
    va->append(quad[3]);

    if (castShadow)
    {
        Vec2 coords;

        coords.x = (quad[0].position.x+quad[2].position.x)*0.5f;
        coords.y = (quad[0].position.y+quad[2].position.y)*0.5f;

        _screenSpaceEntities.push_back(coords);
    }
}

/// Loads and adds a new texture in the render engine
size_t GameRender::registerTexture(std::string filename, bool isRepeated)
{
    _textures.push_back(sf::Texture());

    if (_textures.back().loadFromFile(filename))
    {
        _textures.back().setRepeated(isRepeated);

        _vertices[RenderLayer::RENDER].push_back(sf::VertexArray(sf::Quads, 0));
        _vertices[RenderLayer::GROUND].push_back(sf::VertexArray(sf::Quads, 0));
        _vertices[RenderLayer::BLOOM ].push_back(sf::VertexArray(sf::Quads, 0));

        std::cout << "Add new texture : " << filename << " with ID " << _vertices[RenderLayer::RENDER].size() << std::endl;
    }
    else
    {
        std::cout << "Error : cannot load'" << filename << "'" << std::endl;
    }

    return _textures.size()-1;
}

///
void GameRender::renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target)
{
    float baseOffsetX = _renderSize.x*0.5f;
    float baseOffsetY = _renderSize.y*0.5f;

    baseOffsetX -= _focus.x;
    baseOffsetY -= _focus.y;
    sf::Transform tf;
    tf.translate(baseOffsetX, baseOffsetY);
    sf::RenderStates states;
    states.transform = tf;

    target.draw(va, states);
}

void GameRender::renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target, sf::RenderStates& states)
{
    float baseOffsetX = _renderSize.x*0.5f;
    float baseOffsetY = _renderSize.y*0.5f;

    baseOffsetX -= _focus.x;
    baseOffsetY -= _focus.y;
    sf::Transform tf;
    tf.translate(baseOffsetX, baseOffsetY);
    states.transform = tf;

    target.draw(va, states);
}

/// Draws a vertexArray in the texture
void GameRender::_renderVertices(std::vector<sf::VertexArray>& vertices, sf::RenderTexture& target, sf::RenderStates& states)
{
    size_t size(vertices.size());
    for (size_t i(0); i<size; ++i)
    {
        states.texture = &_textures[i];
        target.draw(vertices[i], states);
        ++_drawCalls;
    }
}

/// Finalizes the textures
void GameRender::display(sf::RenderTarget* target)
{
    float baseOffsetX = _renderSize.x*0.5f;
    float baseOffsetY = _renderSize.y*0.5f;

    baseOffsetX -= _focus.x;
    baseOffsetY -= _focus.y;
    sf::Transform tf;
    tf.translate(baseOffsetX, baseOffsetY);
    sf::RenderStates states;
    states.transform = tf;

    renderGround();

    _renderVertices(_vertices[RenderLayer::RENDER], _renderTexture, states);
    _renderVertices(_vertices[RenderLayer::BLOOM ], _blurTexture  , states);

    /// Draw lights
    sf::Sprite lightSprite(_lightEngine.render());
    _renderTexture.draw(lightSprite, sf::BlendMultiply);

    /// Commented because very costly for modest configurations
    //_renderBloom();

    _renderTexture.display();

    sf::Sprite renderSprite(_renderTexture.getTexture());
    float ratio = 1.0f/_quality;
    renderSprite.setScale(ratio, ratio);
    target->draw(renderSprite);
}

/// Determines if something is in the render view
bool GameRender::isVisible(WorldEntity* entity)
{
    Vec2 coord(entity->getCoord());

    float baseOffsetX = _renderSize.x*0.5f;
    float baseOffsetY = _renderSize.y*0.5f;

    float screenPosX = coord.x-_focus.x;
    float screenPosY = coord.y-_focus.y;

    return (std::abs(screenPosX) < baseOffsetX+2*CELL_SIZE && std::abs(screenPosY) < baseOffsetY+2*CELL_SIZE);
}

bool GameRender::isVisible(const Vec2& position, float radius)
{
    float baseOffsetX = _renderSize.x*0.5f;
    float baseOffsetY = _renderSize.y*0.5f;

    float screenPosX = position.x-_focus.x;
    float screenPosY = position.y-_focus.y;

    return (std::abs(screenPosX) < baseOffsetX+radius && std::abs(screenPosY) < baseOffsetY+radius);
}


const sf::Texture& GameRender::getBlur(const sf::Texture& texture)
{
    return _blur(texture);
}

/// Computes the bloom
void GameRender::_renderBloom()
{
    _blurTexture.display();
    const sf::Texture& bluredTexture = _blur(_blurTexture.getTexture());
    sf::Sprite blurSprite(bluredTexture);
    _renderTexture.draw(blurSprite, sf::BlendAdd);
}

void GameRender::initGround(size_t textureID, sf::VertexArray& quad)
{
    sf::RenderStates states;
    states.texture = &_textures[textureID];
    _groundTexture.draw(quad, states);
}


/// Draw the ground
void GameRender::renderGround()
{
    sf::RenderStates states;
    _renderVertices(_vertices[RenderLayer::GROUND], _groundTexture, states);

    float baseOffsetX = _renderSize.x*0.5f;
    float baseOffsetY = _renderSize.y*0.5f;

    baseOffsetX -= _focus.x;
    baseOffsetY -= _focus.y;

    _groundTexture.display();
    sf::Sprite groundSprite(_groundTexture.getTexture());
    groundSprite.setPosition(baseOffsetX, baseOffsetY);
    groundSprite.setColor(sf::Color(255, 255, 255, 255 ));
    _renderTexture.draw(groundSprite);
}

LightEngine& GameRender::getLightEngine()
{
    return _lightEngine;
}

std::list<Vec2>& GameRender::getScreenSpaceShadowCasters()
{
    return _screenSpaceEntities;
}
