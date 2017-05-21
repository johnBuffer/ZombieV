#ifndef WEAPONSTATICS_HPP_INCLUDED
#define WEAPONSTATICS_HPP_INCLUDED

#include "Weapon.hpp"

template <class T>
class WeaponStatics : public Weapon
{
public:
    static void loadTextures();

protected:
    static sf::Texture __shootTexture;
    static sf::Texture __moveTexture;
    static sf::Texture __reloadTexture;
    static sf::Texture __idleTexture;
};

template <class T>
sf::Texture WeaponStatics<T>::__shootTexture;

template <class T>
sf::Texture WeaponStatics<T>::__moveTexture;

template <class T>
sf::Texture WeaponStatics<T>::__reloadTexture;

template <class T>
sf::Texture WeaponStatics<T>::__idleTexture;

template <class T>
void WeaponStatics<T>::loadTextures()
{
    T::subLoadTextures();
}

#endif // WEAPONSTATICS_HPP_INCLUDED
