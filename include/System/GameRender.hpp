#ifndef GAMERENDER_HPP_INCLUDED
#define GAMERENDER_HPP_INCLUDED

#include "GraphicsUtils.hpp"
#include "Blur/DynamicBlur.h"
#include "LightEngine/LightEngine.hpp"
#include "LightEngine/ShadowCaster.hpp"

#include <list>

enum RenderLayer
{
    RENDER = 0,
    GROUND = 1,
    BLOOM  = 2

};

class GameRender
{
public:
    static void initialize(size_t width, size_t height);
    static void setFocus(const sf::Vector2f& focus);
    static void clear();
    static void addQuad  (size_t textureID, const sf::VertexArray& quad, RenderLayer layer);
    static void display(sf::RenderTarget* target);
    static void initGround(size_t textureID, sf::VertexArray& quad);
    static void renderGround();
    static void fadeGround();

    static void renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target);
    static void renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target, sf::RenderStates states);
    static void renderVertexArray(const sf::VertexArray& va, sf::RenderTexture& target, size_t textureID);
    static void addShadowCaster(const Vec2& position, float radius);

    static bool   isVisible(WorldEntity* entity);
    static bool   isVisible(const Vec2& position, float radius);
    static size_t registerTexture(std::string filename, bool isRepeated=false);
    static const  std::list<ShadowCaster>& getScreenSpaceShadowCasters();

    static LightEngine& getLightEngine();
    static const sf::Texture& getBlur(const sf::Texture& texture);

private:
    GameRender();

    static GameRender* getInstance();

    static GameRender* s_instance;

    static void        _renderVertices(std::vector<sf::VertexArray>& vertices, sf::RenderTexture& target, sf::RenderStates& states);
    float              _quality, _zoom, _ratio;
    size_t             _groundTextureID;
    DynamicBlur        _blur;
    sf::Vector2u       _renderSize;
    sf::Vector2f       _focus;
    sf::Vector2f       _baseOffset;
    sf::VertexArray    _groundQuad;
    sf::RenderTexture  _renderTexture;
    sf::RenderTexture  _blurTexture;
    sf::RenderTexture  _groundTexture;

    static void _renderBloom();
    static void _translateToFocus(sf::Transform& transform);

    std::vector<sf::Texture>                  _textures;
    std::list<ShadowCaster>                   _screenSpaceEntities;
    std::vector<std::vector<sf::VertexArray>> _vertices;

    LightEngine _lightEngine;

    ///Profiling
    static size_t _drawCalls;
};

#endif // GAMERENDER_HPP_INCLUDED
