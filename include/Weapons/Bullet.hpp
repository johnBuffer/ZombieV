#ifndef BULLET_HPP_INCLUDED
#define BULLET_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "System/StandardEntity.hpp"
#include "System/Config.hpp"

class Bullet : public StandardEntity<Bullet>
{
public:
    Bullet();
    Bullet(float angle, float speed, float damage, int penetration);
    Bullet(const Bullet& bullet);
    ~Bullet();

    void initPhysics(GameWorld*) {}
    void init(const Vec2& pos, float angle);
    void update(GameWorld& world);
    void setDone() {_done = true;}
    void render();
    void setImpactForce(float f) {_impact = f;}
    void kill(GameWorld* world);

    const Vec2& getV() const {return _v;}
    Vec2 getImpactForce() const;
    bool isDone() const {return _done;}
    float getDamage() const {return _damage;}
    float getDistance() const {return _distance;}
    int   getPenetration() const {return _penetration;}

    bool collidesWith(WorldEntity* entity) const;

    static void init();

private:
    float _damage;
    float _speed;
    float _recoilForce;
    float _impact;

    int   _penetration;
    int   _drawCount;
    bool  _new;

    Vec2  _v;
    float _distance;

    sf::VertexArray _ownVertexArray;
    static size_t   _textureID;

    bool _done;
};

#endif // BULLET_HPP_INCLUDED
