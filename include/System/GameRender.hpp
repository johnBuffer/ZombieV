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
    const static  std::list<ShadowCaster>& getScreenSpaceShadowCasters();

    static LightEngine& getLightEngine();
    static const sf::Texture& getBlur(const sf::Texture& texture);

private:
    static void               _renderVertices(std::vector<sf::VertexArray>& vertices, sf::RenderTexture& target, sf::RenderStates& states);
    static float              _quality, _zoom, _ratio;
    static sf::Vector2u       _renderSize;
    static sf::Vector2f       _focus;
    static sf::Vector2f       _baseOffset;
    static sf::RenderTexture  _renderTexture;
    static sf::RenderTexture  _blurTexture;
    static sf::RenderTexture  _groundTexture;
    static sf::VertexArray    _groundQuad;
    static DynamicBlur        _blur;
    static size_t             _groundTextureID;

    static void _renderBloom();
    static void _translateToFocus(sf::Transform& transform);

    static std::vector<sf::Texture>                  _textures;
    static std::vector<std::vector<sf::VertexArray>> _vertices;
    static std::list<ShadowCaster>                   _screenSpaceEntities;

    static LightEngine _lightEngine;

    ///Profiling
    static size_t _drawCalls;
};

#endif // GAMERENDER_HPP_INCLUDED
