#include "GameRender.hpp"
#include "Config.hpp"

#include <iostream>

float              GameRender::__quality;
sf::Vector2u       GameRender::__renderSize;
sf::RenderTexture  GameRender::__renderTexture;
sf::RenderTexture  GameRender::__blurTexture;
sf::RenderTexture  GameRender::__groundTexture;

const WorldEntity* GameRender::__focus;
DynamicBlur        GameRender::__blur;

void GameRender::initialize(size_t width, size_t height)
{
    __quality = 0.5;
    __focus = nullptr;
    __renderSize = sf::Vector2u(width*__quality, height*__quality);
    __renderTexture.create(__renderSize.x, __renderSize.y);
    __blurTexture  .create(__renderSize.x, __renderSize.y);
    __groundTexture.create(MAP_SIZE, MAP_SIZE);

    __blur.init(__renderSize.x, __renderSize.y);

    GraphicUtils::init();
}

void GameRender::setFocus(const WorldEntity* body)
{
    __focus = body;
}

void GameRender::clear()
{
    __renderTexture.clear(sf::Color::Black);
    __blurTexture  .clear(sf::Color(0, 0, 0));
    renderGround();
}

void GameRender::render(GraphicEntity entity)
{
    double baseOffsetX = __renderSize.x/2;
    double baseOffsetY = __renderSize.y/2;

    if (__focus)
    {
        baseOffsetX -= __focus->getCoord().x;
        baseOffsetY -= __focus->getCoord().y;
    }

    sf::Transform tf;
    tf.translate(baseOffsetX, baseOffsetY);
    tf *= entity.getTransform();

    sf::RenderStates states;
    states.texture   = entity._texture;
    states.transform = tf;

    if (entity._target == RENDER)
    {
        __renderTexture.draw(*entity._vertexArray, states);
    }
    else if (entity._target == GROUND)
    {
        states.transform = sf::Transform::Identity;
        __groundTexture.draw(*entity._vertexArray, states);
    }
    else if (entity._target == BLOOM)
    {
        __renderTexture.draw(*entity._vertexArray, states);
        states.blendMode = sf::BlendAdd;
        __blurTexture.draw(*entity._vertexArray, states);
    }
}

void GameRender::render(std::list<GraphicEntity>& entities)
{
    double baseOffsetX = __renderSize.x/2;
    double baseOffsetY = __renderSize.y/2;

    if (__focus)
    {
        baseOffsetX -= __focus->getCoord().x;
        baseOffsetY -= __focus->getCoord().y;
    }

    for (GraphicEntity entity : entities)
    {
        sf::Transform tf;
        tf.translate(baseOffsetX, baseOffsetY);
        tf *= entity.getTransform();

        sf::RenderStates states;
        states.texture   = entity._texture;
        states.transform = tf;

        if (entity._target == RENDER)
        {
            __renderTexture.draw(*entity._vertexArray, states);
        }
        else if (entity._target == GROUND)
        {
            states.transform = sf::Transform::Identity;
            __groundTexture.draw(*entity._vertexArray, states);
        }
        else if (entity._target == BLOOM)
        {
            __renderTexture.draw(*entity._vertexArray, states);
            states.blendMode = sf::BlendAdd;
            __blurTexture.draw(*entity._vertexArray, states);
        }
    }
}

void GameRender::display(sf::RenderTarget* target)
{
    __renderBloom();
    __renderTexture.display();

    sf::Sprite renderSprite(__renderTexture.getTexture());
    float ratio = 1/__quality;
    renderSprite.setScale(ratio, ratio);
    target->draw(renderSprite);
}

bool GameRender::isVisible(WorldEntity* entity)
{
    U_2DCoord coord(entity->getCoord());

    double baseOffsetX = __renderSize.x/2;
    double baseOffsetY = __renderSize.y/2;

    double screenPosX = coord.x-__focus->getCoord().x;
    double screenPosY = coord.y-__focus->getCoord().y;

    return (std::abs(screenPosX) < baseOffsetX+2*CELL_SIZE && std::abs(screenPosY) < baseOffsetY+2*CELL_SIZE);
}

void GameRender::__renderBloom()
{
    __blurTexture.display();
    const sf::Texture& bluredTexture = __blur(__blurTexture.getTexture());
    sf::Sprite blurSprite(bluredTexture);
    __renderTexture.draw(blurSprite, sf::BlendAdd);
}

void GameRender::renderGround()
{
    double baseOffsetX = __renderSize.x/2;
    double baseOffsetY = __renderSize.y/2;

    if (__focus)
    {
        baseOffsetX -= __focus->getCoord().x;
        baseOffsetY -= __focus->getCoord().y;
    }

    __groundTexture.display();
    sf::Sprite groundSprite(__groundTexture.getTexture());
    groundSprite.setPosition(baseOffsetX, baseOffsetY);
    groundSprite.setColor(sf::Color(255, 255, 255, 225));
    __renderTexture.draw(groundSprite);
}
