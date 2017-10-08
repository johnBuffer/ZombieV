#ifndef WEAPONSTATICS_HPP_INCLUDED
#define WEAPONSTATICS_HPP_INCLUDED

#include "Weapon.hpp"
#include "System/DrawableEntity.hpp"

template <class T>
class WeaponStatics : public Weapon, public DrawableEntity<T>
{
protected:
    static size_t _shootTextureID;
    static size_t _moveTextureID;
    static size_t _reloadTextureID;
    static size_t _idleTextureID;

    static std::vector<size_t> _shootSoundsID;
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
std::vector<size_t> WeaponStatics<T>::_shootSoundsID;

#endif // WEAPONSTATICS_HPP_INCLUDED
