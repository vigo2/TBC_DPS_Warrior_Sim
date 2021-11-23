#ifndef WOW_SIMULATOR_ABILITY_HPP
#define WOW_SIMULATOR_ABILITY_HPP

#include "damage_sources.hpp"
#include "sim_state.hpp"
#include "hit_result.hpp"
#include "hit_outcome.hpp"

struct Ability {
    const std::string name;
    const Damage_source damage_source;

    int gcd;
    int cooldown;
    //int next_cooldown;

    double cost;
    double refund;

    std::function<double(Sim_state&)> damage{};
    std::function<void()> cast{};
    std::function<void(Sim_state&, const Hit_outcome&)> after_cast{};

    // maybe more - e.g. can_sweep, damage_func(), refund()

    // casts - hit-results

    // this could also keep the per-spell/ability damage and other stats
    // e.g. min-max-avg-count, plus hit_results (miss/dodge/glance/crit/hit),
    // plus rage-stats (rage spend/rage gained)
};

#endif // WOW_SIMULATOR_ABILITY_HPP
