#ifndef U2DCOORD_H_INCLUDED
#define U2DCOORD_H_INCLUDED

class U_2DCoord
{
public:
    U_2DCoord();
    U_2DCoord(const U_2DCoord&, const U_2DCoord&);
    U_2DCoord(double x, double y);

    void move2D(U_2DCoord& vec);
    double getNorm() const;

    double x, y;

};

U_2DCoord operator/(const U_2DCoord&, double);

//U_2DCoord operator-(U_2DCoord, U_2DCoord);
U_2DCoord operator-(const U_2DCoord&, const U_2DCoord&);
U_2DCoord operator+(const U_2DCoord&, const U_2DCoord&);
U_2DCoord operator-(const U_2DCoord&);
U_2DCoord operator*(const U_2DCoord&, double d);


#endif // U2DCOORD_H_INCLUDED
