#include "Attributes.hpp"

double multiplicative_addition(double val1, double val2)
{
    return (1 + val1) * (1 + val2) - 1;
}

double multiplicative_subtraction(double val1, double val2)
{
    return (1 + val1) / (1 + val2) - 1;
}

std::ostream& operator<<(std::ostream& os, Attributes const& a)
{
    os << "strength = " << a.strength << std::endl;
    os << "agility = " << a.agility << std::endl;
    return os;
}


std::ostream& operator<<(std::ostream& os, const Special_stats& ss)
{
    os << "attack power = " << ss.attack_power << std::endl;
    os << "crit = " << ss.critical_strike << std::endl;
    if (ss.hit > 0) os << "hit = " << ss.hit << std::endl;
    if (ss.expertise > 0) os << "expertise = " << ss.expertise << std::endl;
    if (ss.haste > 0) os << "haste = " << ss.haste << std::endl;
    if (ss.gear_armor_pen > 0) os << "arpen = " << ss.gear_armor_pen << std::endl;
    // this covers about 99% of all cases
    return os;
}