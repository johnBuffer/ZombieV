#ifndef FIRE_HPP_INCLUDED
#define FIRE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "System/StandardEntity.hpp"

class Fire : public StandardEntity<Fire>
{
public:
    Fire();
    Fire(Vec2 pos, float angle, float sizeFactor=1.0f);

    void initPhysics(GameWorld*) {}
    void update(GameWorld& world);
    void render();
    bool isDone() const {return _isDone;}

    void kill(GameWorld* world);

    static void init();

private:
    static size_t   _textureID;
    sf::VertexArray _vertexArray;

    bool  _isDone;
    float _sizeFactor;
};

#endif // FIRE_HPP_INCLUDED
