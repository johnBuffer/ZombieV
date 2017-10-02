#include "Weapons/Weapon.hpp"

Weapon::Weapon()
{
}

bool Weapon::isReady() const
{
    return _currentFireCooldown <= 0 && _currentAmmo;
}
