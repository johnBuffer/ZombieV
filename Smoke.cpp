#include "Smoke.hpp"
#include "GameWorld.hpp"

sf::VertexArray Smoke::__vertexArray;
sf::Texture     Smoke::__texture;

void Smoke::init()
{
    __vertexArray = sf::VertexArray(sf::Quads, 0);
    __texture.loadFromFile("data/textures/smoke2.png");
}

Smoke::Smoke()
{

}

Smoke::Smoke(U_2DCoord pos, U_2DCoord vel, float angularVel, float size) :
    __ratio(0.0f),
    __size(size),
    __angularVelocity(angularVel),
    __velocity(vel),
    __isDone(false)
{
    __body.setPosition(pos);

    float a(rand()%3600/10.0f);
    __angle = a*DEGRAD;
}

void Smoke::update(GameWorld& world)
{
    __ratio += 4.0f*0.016f;
    __angle += __angularVelocity*(1.0f-__ratio);
    __body.move2D(__velocity);

    __isDone = __ratio>1.0f;
}

void Smoke::render()
{
    float ratio = __ratio+0.25;
    float sx   = ratio*__size*cos(__angle);
    float sy   = ratio*__size*sin(__angle);

    U_2DCoord coord(getCoord());
    float x = coord.x;
    float y = coord.y;

    sf::Color color(50, 50, 50, (1.0f-__ratio)*200.f);

    sf::Vertex v1(sf::Vector2f(x+sx, y+sy), color, sf::Vector2f(0 , 0));
    sf::Vertex v2(sf::Vector2f(x+sy, y-sx), color, sf::Vector2f(256, 0));
    sf::Vertex v3(sf::Vector2f(x-sx, y-sy), color, sf::Vector2f(256, 256));
    sf::Vertex v4(sf::Vector2f(x-sy, y+sx), color, sf::Vector2f(0  , 256));

    __vertexArray.append(v1);
    __vertexArray.append(v2);
    __vertexArray.append(v3);
    __vertexArray.append(v4);
}
