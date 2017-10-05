#ifndef U2DCOORD_H_INCLUDED
#define U2DCOORD_H_INCLUDED

class Vec2
{
public:
    Vec2();
    Vec2(const Vec2&, const Vec2&);
    Vec2(float x, float y);

    void move2D(const Vec2& vec);
    float getNorm()  const;
    float getNorm2() const;

    float x, y;

};

Vec2 operator/(const Vec2&, float);

//Vec2 operator-(Vec2, Vec2);
Vec2 operator-(const Vec2&, const Vec2&);
Vec2 operator+(const Vec2&, const Vec2&);
Vec2 operator-(const Vec2&);
Vec2 operator*(const Vec2&, float d);


#endif // U2DCOORD_H_INCLUDED
