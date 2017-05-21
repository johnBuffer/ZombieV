#include "Zombie.hpp"
#include "GameWorld.hpp"
#include "GameRender.hpp"

#include <iostream>

sf::Texture Zombie::__moveTexture;
Animation   Zombie::__moveAnimation(3, 6, 288, 311, 17, 20);

Zombie::Zombie(double x, double y)
{
    __body.setX(x);
    __body.setY(y);
    __body.stop();

    __speed = 150;
    __life  = 100;

    std::cout << "Zombie at " << x << " " << y << std::endl;

    __currentAnimation = __moveAnimation;

    int c=rand()%50+200;
    __color=sf::Color(c, c, c);

    __time = rand()%100;
    __isEnnemy = true;
}

void Zombie::setTarget(WorldEntity* target)
{
    __target = target;
}

void Zombie::update(GameWorld& world)
{
    __time += 0.016;

    double vx = __target->getCoord().x - __body.getPosition().x;
    double vy = __target->getCoord().y - __body.getPosition().y;
    double dist = sqrt(vx*vx+vy*vy);
    __angle = acos(vx/dist)+PI;
    __angle = vy < 0 ? -__angle : __angle;

    vx /= dist;
    vy /= dist;

    double speed = 10;

    getBody().accelerate2D(U_2DCoord(speed*vx, speed*vy));
}

void Zombie::render()
{
    double x = __body.getPosition().x;
    double y = __body.getPosition().y;

    double radius = CELL_SIZE*2.2;

    sf::Vector2f corner1(x+radius*cos(__angle+  PI/4.0f), y+radius*sin(__angle+  PI/4.0f));
    sf::Vector2f corner2(x+radius*cos(__angle+3*PI/4.0f), y+radius*sin(__angle+3*PI/4.0f));
    sf::Vector2f corner3(x+radius*cos(__angle-3*PI/4.0f), y+radius*sin(__angle-3*PI/4.0f));
    sf::Vector2f corner4(x+radius*cos(__angle-  PI/4.0f), y+radius*sin(__angle-  PI/4.0f));

    sf::IntRect texCoords = __currentAnimation.getTexCoord(__time);
    float textureX = texCoords.left;
    float textureY = texCoords.top;

    sf::Vertex vertex1(corner1, __color, sf::Vector2f(textureX                , textureY));
    sf::Vertex vertex2(corner2, __color, sf::Vector2f(textureX+texCoords.width, textureY));
    sf::Vertex vertex3(corner3, __color, sf::Vector2f(textureX+texCoords.width, textureY+texCoords.height));
    sf::Vertex vertex4(corner4, __color, sf::Vector2f(textureX                , textureY+texCoords.height));

    __vertexArray.append(vertex1);
    __vertexArray.append(vertex2);
    __vertexArray.append(vertex3);
    __vertexArray.append(vertex4);
}

void Zombie::loadTexture()
{
    __moveTexture.loadFromFile("data/textures/zombie/zombie_move.png");
    __moveAnimation.setTexture(__moveTexture);
}

