#ifndef WOW_SIMULATOR_HIT_OUTCOME_HPP
#define WOW_SIMULATOR_HIT_OUTCOME_HPP

#include "hit_result.hpp"

struct Hit_outcome
{
    Hit_outcome() : damage(0), hit_result(Hit_result::TBD), rage_damage(0) {}
    Hit_outcome(double damage, Hit_result hit_result) : damage(damage), hit_result(hit_result), rage_damage(damage) {}
    Hit_outcome(double damage, Hit_result hit_result, double rage_damage) : damage(damage), hit_result(hit_result), rage_damage(rage_damage) {}

    double damage;
    Hit_result hit_result;
    double rage_damage;

    [[nodiscard]] bool isDodgeOrMiss() const { return hit_result & (Hit_result::miss | Hit_result::dodge); }
};

#endif // WOW_SIMULATOR_HIT_OUTCOME_HPP
