#ifndef BULLET_HPP_INCLUDED
#define BULLET_HPP_INCLUDED

#include "WorldEntity.hpp"
#include "MassiveDrawableEntity.hpp"
#include "Config.hpp"

class Bullet : public WorldEntity, public MassiveDrawableEntity<Bullet>
{
public:
    Bullet();
    Bullet(double angle, double speed, double damage, int penetration);

    void init(U_2DCoord pos, double angle);
    void update(GameWorld& world);
    void setDone() {__done = true;}
    void render();
    void setOffset(double d) {__offset=d;}

    U_2DCoord getV() const {return U_2DCoord(__vx, __vy);}
    bool isDone()    const {return __done;}

    static void          loadTexture();
    static GraphicEntity subGetGraphicEntity() {return GraphicEntity(__vertexArray, __texture, BLOOM);}
    static void          subResetVertexArray() {__vertexArray.clear();}

    bool collidesWith(WorldEntity* entity) const;

private:
    double __damage;
    double __speed;
    int    __penetration;

    double __vx;
    double __vy;
    double __distance;

    double __offset;

    sf::VertexArray        __ownVertexArray;
    static sf::Texture     __texture;

    bool __done;
};

#endif // BULLET_HPP_INCLUDED
