#include "Weapons/Weapon.hpp"

Weapon::Weapon()
{
}

bool Weapon::isReady() const
{
    return m_fireCooldown.isReady() && _currentAmmo;
}
