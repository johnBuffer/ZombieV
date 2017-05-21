#ifndef GAMERENDER_HPP_INCLUDED
#define GAMERENDER_HPP_INCLUDED

#include "GameWorld.hpp"
#include "GraphicEntity.hpp"
#include "GraphicsUtils.hpp"
#include "Blur/DynamicBlur.h"

class GameRender
{
public:
    static void initialize(size_t width, size_t height);
    static void setFocus(const WorldEntity* entity);
    static void clear();

    static void render(GraphicEntity entity);
    static void render(std::list<GraphicEntity>& entities);

    static void display(sf::RenderTarget* target);
    static bool isVisible(WorldEntity* entity);
    static void renderGround();


private:
    static float              __quality;
    static sf::Vector2u       __renderSize;
    static sf::RenderTexture  __renderTexture;
    static sf::RenderTexture  __blurTexture;
    static sf::RenderTexture  __groundTexture;

    static const WorldEntity* __focus;

    static DynamicBlur __blur;
    static void        __renderBloom();
};

#endif // GAMERENDER_HPP_INCLUDED
