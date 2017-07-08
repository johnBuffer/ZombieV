#ifndef WOODBOX_HPP_INCLUDED
#define WOODBOX_HPP_INCLUDED

#include "WorldEntity.hpp"

class WoodBox : WorldEntity
{
public:
    WoodBox();

private:
    U_2DBody _c1;
    U_2DBody _c2;
    U_2DBody _c3;
    U_2DBody _c4;
};

#endif // WOODBOX_HPP_INCLUDED
