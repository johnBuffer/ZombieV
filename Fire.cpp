#include "Fire.hpp"
#include "Config.hpp"

sf::Texture Fire::__texture;

void Fire::init()
{
    Fire::__texture.loadFromFile("data/textures/fire.png");
}

Fire::Fire()
{

}

Fire::Fire(U_2DCoord pos, float angle):
    __vertexArray(sf::Quads, 4)
{
    __body.setPosition(pos);
    __angle = angle;
}

GraphicEntity Fire::getGraphicEntity()
{
    U_2DCoord pos(getCoord());
    float length = 50;
    float rectA  = PI/3.50;

    __vertexArray[0].position = sf::Vector2f(pos.x+length*cos(__angle+rectA)   , pos.y+length*sin(__angle+rectA));
    __vertexArray[1].position = sf::Vector2f(pos.x+length*cos(__angle+PI-rectA), pos.y+length*sin(__angle+PI-rectA));
    __vertexArray[2].position = sf::Vector2f(pos.x+length*cos(__angle+PI+rectA), pos.y+length*sin(__angle+PI+rectA));
    __vertexArray[3].position = sf::Vector2f(pos.x+length*cos(__angle-rectA)   , pos.y+length*sin(__angle-rectA));

    int h = rand()%8;

    int tx = (h%4)*64;
    int ty = (h/4)*64;

    __vertexArray[0].texCoords = sf::Vector2f(tx   , ty);
    __vertexArray[1].texCoords = sf::Vector2f(tx+64, ty);
    __vertexArray[2].texCoords = sf::Vector2f(tx+64, ty+64);
    __vertexArray[3].texCoords = sf::Vector2f(tx   , ty+64);

    return GraphicEntity(__vertexArray, __texture, BLOOM);
}

void Fire::update(GameWorld& world)
{

}

void Fire::render()
{

}

