#ifndef WOW_SIMULATOR_WEAPON_SIM_HPP
#define WOW_SIMULATOR_WEAPON_SIM_HPP

#include "include/Item.hpp"

class Weapon_sim
{
public:
    Weapon_sim(const Weapon& weapon, const Special_stats& special_stats);

    [[nodiscard]] double swing(double attack_power) const
    {
        return average_damage + attack_power * swing_speed / 14;
    }

    [[nodiscard]] double normalized_swing(double attack_power) const
    {
        return average_damage + attack_power * normalized_swing_speed / 14;
    }

    const double swing_speed;
    double normalized_swing_speed;
    double next_swing;
    double min_damage;
    double max_damage;
    double average_damage;
    Socket socket;
    Weapon_type weapon_type;
    Weapon_socket weapon_socket;
    std::vector<Hit_effect> hit_effects;
    std::string socket_name;
};

#endif // WOW_SIMULATOR_WEAPON_SIM_HPP
