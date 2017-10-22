#ifndef WEAPONSTATICS_HPP_INCLUDED
#define WEAPONSTATICS_HPP_INCLUDED

#include "Weapon.hpp"

template <class T>
class WeaponStatics : public Weapon
{
protected:
    static size_t _shootTextureID;
    static size_t _moveTextureID;
    static size_t _reloadTextureID;
    static size_t _idleTextureID;

    static size_t _shootSoundID;
};

template <class T>
size_t WeaponStatics<T>::_shootTextureID;

template <class T>
size_t WeaponStatics<T>::_moveTextureID;

template <class T>
size_t WeaponStatics<T>::_reloadTextureID;

template <class T>
size_t WeaponStatics<T>::_idleTextureID;

template <class T>
size_t WeaponStatics<T>::_shootSoundID;

#endif // WEAPONSTATICS_HPP_INCLUDED
