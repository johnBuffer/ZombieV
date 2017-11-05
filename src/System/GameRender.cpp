#include "System/GameRender.hpp"
#include "System/Config.hpp"

#include <iostream>

float              GameRender::_quality;
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
LightEngine GameRender::_lightEngine;

void GameRender::initialize(size_t width, size_t height)
{
    _quality = 1.f;
    _zoom    = 1.75f;

    _ratio = _zoom/_quality;
    _focus = sf::Vector2f(0.0, 0.0);
    _renderSize = sf::Vector2u(width*_quality, height*_quality);

    float bx = _renderSize.x/(_quality*_zoom)*0.5f;
    float by = _renderSize.y/(_quality*_zoom)*0.5f;
    _baseOffset = sf::Vector2f(bx, by);

    _renderTexture.create(_renderSize.x, _renderSize.y);
    _blurTexture.create(_renderSize.x, _renderSize.y);
    _groundTexture.create(MAP_SIZE, MAP_SIZE);

    _blur.init(_renderSize.x*0.5f, _renderSize.y*0.5f);
    _blur.setDownSizeFactor(2);
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
    renderVertexArray(_groundQuad, _renderTexture, _groundTextureID);
    _screenSpaceEntities.clear();
    _lightEngine.clear();
}

/// Adds a quad to be rendered in the current frame
void GameRender::addQuad(size_t textureID, const sf::VertexArray& quad, RenderLayer layer)
{
    sf::VertexArray* va(&_vertices[layer][textureID]);
    va->append(quad[0]);
    va->append(quad[1]);
    va->append(quad[2]);
    va->append(quad[3]);
}

void GameRender::addShadowCaster(const Vec2& position, float radius)
{
    _screenSpaceEntities.push_back(ShadowCaster(position, radius));
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
    sf::RenderStates states;
    _translateToFocus(states.transform);

    target.draw(va, states);
}

void GameRender::renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target, sf::RenderStates states)
{
    _translateToFocus(states.transform);

    target.draw(va, states);
}

void GameRender::renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target, size_t textureID)
{
    sf::RenderStates states;
    _translateToFocus(states.transform);
    states.texture = &_textures[textureID];

    target.draw(va, states);
}

/// Draws a vertexArray in the texture
void GameRender::_renderVertices(std::vector<sf::VertexArray>& vertices, sf::RenderTexture& target, sf::RenderStates& states)
{
    size_t size(vertices.size());
    for (size_t i(0); i<size; ++i)
    {
        if (vertices[i].getVertexCount())
        {
            states.texture = &_textures[i];
            target.draw(vertices[i], states);
            ++_drawCalls;
        }
    }
}

/// Finalizes the textures
void GameRender::display(sf::RenderTarget* target)
{
    sf::Transform tf;
    tf.scale(_quality, _quality);
    _translateToFocus(tf);
    sf::RenderStates states;
    states.transform = tf;

    renderGround();

    _renderVertices(_vertices[RenderLayer::RENDER], _renderTexture, states);

    /// Draw lights
    sf::Sprite lightSprite(_lightEngine.render());
    //_renderTexture.draw(lightSprite, sf::BlendMultiply);
    _renderTexture.display();

    sf::Sprite renderSprite(_renderTexture.getTexture());
    renderSprite.setScale(_ratio, _ratio);
    target->draw(renderSprite);
}

/// Determines if something is in the render view
bool GameRender::isVisible(WorldEntity* entity)
{
    return isVisible(entity->getCoord(), 2*CELL_SIZE);
}

bool GameRender::isVisible(const Vec2& position, float radius)
{
    float screenPosX = position.x-_focus.x;
    float screenPosY = position.y-_focus.y;

    return (std::abs(screenPosX) < _baseOffset.x+radius && std::abs(screenPosY) < _baseOffset.y+radius);
}


const sf::Texture& GameRender::getBlur(const sf::Texture& texture)
{
    return _blur(texture);
}

void GameRender::initGround(size_t textureID, sf::VertexArray& quad)
{
    _groundTextureID = textureID;
    _groundQuad = quad;
    _groundTexture.clear(sf::Color(0.0, 0.0, 0.0, 0.0));
}


/// Draw the ground
void GameRender::renderGround()
{
    sf::RenderStates states;
    _renderVertices(_vertices[RenderLayer::GROUND], _groundTexture, states);

    _groundTexture.display();
    sf::Sprite groundSprite(_groundTexture.getTexture());
    groundSprite.setScale(_quality, _quality);
    float tx = (_focus.x-_baseOffset.x)*_quality;
    float ty = (_focus.y-_baseOffset.y)*_quality;

    groundSprite.setPosition(-tx, -ty);
    //groundSprite.setColor(sf::Color(255, 255, 255, 255));
    _renderTexture.draw(groundSprite, states);
}

void GameRender::fadeGround()
{
    sf::Vector2i size(_groundTexture.getSize());
    sf::RectangleShape rectangle(sf::Vector2f(size.x, size.y));
    rectangle.setFillColor(sf::Color(255, 255, 255, 254));
    _groundTexture.draw(rectangle, sf::BlendMultiply);
}

void GameRender::_translateToFocus(sf::Transform& transform)
{
    transform.translate(_baseOffset.x-_focus.x, _baseOffset.y-_focus.y);
}

LightEngine& GameRender::getLightEngine()
{
    return _lightEngine;
}

const std::list<ShadowCaster>& GameRender::getScreenSpaceShadowCasters()
{
    return _screenSpaceEntities;
}
