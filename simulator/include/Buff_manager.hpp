#ifndef WOW_SIMULATOR_BUFF_MANAGER_HPP
#define WOW_SIMULATOR_BUFF_MANAGER_HPP

#include "Attributes.hpp"
#include "Item.hpp"

#include "time_keeper.hpp"
#include "damage_sources.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_map>
#include <vector>

struct Over_time_buff
{
    Over_time_buff(const Over_time_effect& effect, double current_time)
        : name(effect.name)
        , rage_gain(effect.rage_gain)
        , damage(effect.damage)
        , special_stats(effect.special_stats)
        , interval(effect.interval)
        , next_tick(current_time + effect.interval)
        , next_fade(current_time + effect.duration) {}

    std::string name;

    double rage_gain;
    double damage;
    Special_stats special_stats;

    double interval;

    double next_tick;
    double next_fade;
};

struct Combat_buff
{
    Combat_buff(const Hit_effect& hit_effect, const Special_stats& multipliers, double current_time) :
        name(hit_effect.name),
        special_stats_boost(hit_effect.special_stats_boost + hit_effect.attribute_boost.convert_to_special_stats(multipliers)),
        next_fade(current_time + hit_effect.duration),
        stacks(1),
        uptime(0),
        last_gain(current_time) {}

    const std::string name;
    const Special_stats special_stats_boost;
    double next_fade;
    int stacks;

    // statistics
    double uptime;
    double last_gain;
};

struct Hit_buff
{
    Hit_buff(std::string id, double duration_left) : id(std::move(id)), duration_left(duration_left){};

    std::string id;
    double duration_left;
};

class Buff_manager
{
public:
    // CHECKME this is called _per sim run_, and resets a lot more than actually necessary
    void initialize(Special_stats& special_stats, Damage_sources& damage_sources,
                    const std::vector<std::pair<double, Use_effect>>& use_effects_order,
                    std::vector<Hit_effect>& hit_effects_mh_input, std::vector<Hit_effect>& hit_effects_oh_input,
                    double tactical_mastery_rage)
    {
        simulation_special_stats = &special_stats;
        simulation_damage_sources = &damage_sources;

        combat_buffs.clear();
        hit_gains.clear();
        over_time_buffs.clear();
        hit_effects_mh = &hit_effects_mh_input;
        hit_effects_oh = &hit_effects_oh_input;
        use_effect_order = use_effects_order;
        need_to_recompute_hit_tables = true;
        tactical_mastery_rage_ = tactical_mastery_rage;
    };

    void update_aura_uptimes(double current_time) {
        for (const auto& buff : combat_buffs)
        {
            auto uptime = buff.uptime;
            if (buff.stacks > 0) uptime += current_time - buff.last_gain;
            aura_uptime[buff.name] += uptime;
        }
    }

    [[nodiscard]] double get_dt(double current_time) const
    {
        auto dt = std::numeric_limits<double>::max();
        if (min_combat_buff >= current_time && min_combat_buff < dt) dt = min_combat_buff;
        if (min_over_time_buff >= current_time && min_over_time_buff < dt) dt = min_over_time_buff;
        return dt - current_time;
    }

    void increment_combat_buffs(double current_time, double& rage, double& rage_lost_stance, bool debug,
                              std::vector<std::string>& debug_msg)
    {
        if (min_combat_buff >= current_time)
        {
            return;
        }

        min_combat_buff = std::numeric_limits<double>::max();
        for (auto& buff : combat_buffs)
        {
            if (buff.stacks == 0) // inactive
            {
                continue;
            }

            if (buff.next_fade >= current_time) // not ready
            {
                min_combat_buff = std::min(min_combat_buff, buff.next_fade);
                continue;
            }

            const auto& ssb = buff.special_stats_boost;
            for (int i = 0; i < buff.stacks; i++)
            {
                (*simulation_special_stats) -= ssb;
            }
            buff.stacks = 0;
            need_to_recompute_hit_tables |= (ssb.critical_strike > 0 || ssb.hit > 0 || ssb.expertise > 0);
            need_to_recompute_mitigation |= (ssb.gear_armor_pen > 0);

            // special case, should be removed
            if (buff.name == "battle_stance")
            {
                if (rage > tactical_mastery_rage_)
                {
                    rage_lost_stance += rage - tactical_mastery_rage_;
                    rage = tactical_mastery_rage_;
                }
            }

            buff.uptime += buff.next_fade - buff.last_gain;

            if (debug)
            {
                debug_msg.emplace_back(buff.name + " fades.");
            }
        }

        /*
        int stacks = 1;
        for (auto it = combat_buffs.begin(); it != combat_buffs.end();)
        {
            const auto& name = it->first;
            auto& buff = it->second;

            if (!performance_mode && current_time > 0.0)
            {
                aura_uptime[name] += dt;
            }

            buff.duration_left -= dt;
            if (buff.duration_left < 0.0)
            {
                if (debug)
                {
                    debug_msg.emplace_back(name + " fades.");
                }
                if (name == "battle_stance")
                {
                    if (rage > tactical_mastery_rage_)
                    {
                        rage_lost_stance += rage - tactical_mastery_rage_;
                        rage = tactical_mastery_rage_;
                    }
                }

                if (buff.is_independent_stacks)
                {
                    stacks = 0;
                    for (auto& hit_effect_mh : (*hit_effects_mh))
                    {
                        if (name == hit_effect_mh.name)
                        {
                            stacks = hit_effect_mh.stacks_counter;
                            hit_effect_mh.stacks_counter = 0;
                            break;
                        }
                    }
                    if (!(stacks > 0))
                    {
                        for (auto& hit_effect_oh : (*hit_effects_oh))
                        {
                            if (name == hit_effect_oh.name)
                            {
                                stacks = hit_effect_oh.stacks_counter;
                                hit_effect_oh.stacks_counter = 0;
                                break;
                            }
                        }
                    }
                }
                else if (buff.is_shared_stacks)
                {
                    stacks = arpen_stacks_counter;
                    arpen_stacks_counter = 0;
                    need_to_recompute_mitigation = true;
                    // for (auto& hit_effects_mh : (*hit_effects_mh))
                    // {
                    //     if (it->first == hit_effects_mh.name)
                    //     {
                    //         stacks = hit_effects_mh.stacks_counter;
                    //         hit_effects_mh.stacks_counter = 0;
                    //         break;
                    //     }
                    // }
                    // for (auto& hit_effects_oh : (*hit_effects_oh))
                    // {
                    //     if (it->first == hit_effects_oh.name)
                    //     {
                    //         stacks = hit_effects_oh.stacks_counter;
                    //         hit_effects_oh.stacks_counter = 0;
                    //         break;
                    //     }
                    // }
                }

                for (int i = 0; i < stacks; i++)
                {
                    (*simulation_special_stats) -= buff.special_stats;
                }
                // if (it->second.special_stats.gear_armor_pen > 0)
                // {
                //     need_to_recompute_mitigation = true;
                // }
                if (buff.special_stats.hit > 0 || buff.special_stats.critical_strike > 0 || buff.special_stats.expertise > 0)
                {
                    need_to_recompute_hit_tables = true;
                }
                it = combat_buffs.erase(it);
            }
            else
            {
                next_event = std::min(next_event, buff.duration_left);
                ++it;
            }
        }
        */
    }

    void increment_hit_gains(double current_time, double dt, bool debug, std::vector<std::string>& debug_msg)
    {
        exit(44);

        double next_event;
        for (auto it = hit_gains.begin(); it != hit_gains.end();)
        {
            auto& buff = *it;
            if (current_time > dt) dt = dt + 0 * current_time;
            //if (!performance_mode && current_time > 0.0)
            //{
            //    aura_uptime[buff.id] += dt;
            //}
            buff.duration_left -= dt;
            if (buff.duration_left < 0.0)
            {
                if (debug)
                {
                    debug_msg.emplace_back(buff.id + " fades.");
                }

                for (auto jt = hit_effects_mh->begin(); jt != hit_effects_mh->end();)
                {
                    jt->name == buff.id ? jt = hit_effects_mh->erase(jt) : ++jt;
                }
                for (auto jt = hit_effects_oh->begin(); jt != hit_effects_oh->end();)
                {
                    jt->name == buff.id ? jt = hit_effects_oh->erase(jt) : ++jt;
                }

                //reset_armor_reduction = true;
                it = hit_gains.erase(it);
            }
            else
            {
                next_event = std::min(next_event, buff.duration_left);
                ++it;
            }
        }
    }

    void increment_use_effects(double current_time, double& rage, Time_keeper& time_keeper, bool debug,
                               std::vector<std::string>& debug_msg, double ap_multiplier)
    {
        // Try to use the use effect within one second of what was planned (compensate some for GCD's)
        double margin = 0.0;
        if (current_time > 0.0)
        {
            margin = 1.0;
        }
        if (!use_effect_order.empty() && current_time > use_effect_order.back().first - margin)
        {
            auto& use_effect = use_effect_order.back().second;
            if (!use_effect.triggers_gcd || time_keeper.global_ready())
            {
                if (debug)
                {
                    debug_msg.emplace_back("Activating: " + use_effect.name);
                }
                if (ap_multiplier > 0)
                {
                    if (use_effect.name == "unleashed_rage")
                    {
                        use_effect.special_stats_boost.attack_power += use_effect.special_stats_boost.attack_power * (ap_multiplier - 0.1);
                    }
                    else
                    {
                        use_effect.special_stats_boost.attack_power += use_effect.special_stats_boost.attack_power * ap_multiplier;
                    }
                }
                if (!use_effect.hit_effects.empty())
                {
                    add_hit_effect(use_effect.name, use_effect.hit_effects[0], use_effect.hit_effects[0].duration);
                }
                else if (!use_effect.over_time_effects.empty())
                {
                    add_over_time_buff(use_effect.over_time_effects[0], current_time);
                }
                else
                {
                    Hit_effect hit_effect;
                    hit_effect.name = use_effect.name;
                    hit_effect.special_stats_boost = use_effect.get_special_stat_equivalent(*simulation_special_stats, ap_multiplier);
                    hit_effect.duration = use_effect.duration;
                    add_combat_buff(hit_effect, current_time);
                }
                if (use_effect.rage_boost != 0.0)
                {
                    rage += use_effect.rage_boost;
                    rage = std::min(100.0, rage);
                    if (debug)
                    {
                        debug_msg.emplace_back("Current rage: " + std::to_string(int(rage)));
                    }
                }
                if (use_effect.triggers_gcd)
                {
                    time_keeper.global_cast(1.5);
                }
                use_effect_order.pop_back();
            }
        }
    }

    void increment_over_time_buffs(double current_time, double& rage, bool debug, std::vector<std::string>& debug_msg)
    {
        if (min_over_time_buff >= current_time)
        {
            return;
        }

        min_over_time_buff = std::numeric_limits<double>::max();
        for (auto& buff : over_time_buffs)
        {
            if (buff.next_tick >= current_time) // not ready
            {
                min_over_time_buff = std::min(min_over_time_buff, buff.next_tick);
                continue;
            }

            // this used to support everything at once, but no over_time_buff actually granted rage, dealt damage, and added stats.
            //  nothing does the latter, afaik
            if (buff.rage_gain > 0)
            {
                rage += buff.rage_gain;
                rage = std::min(100.0, rage);
                if (debug)
                {
                    debug_msg.emplace_back("Over time effect: " + buff.name + " tick. Current rage: " + std::to_string(int(rage)));
                }
            }
            else if (buff.damage > 0)
            {
                simulation_damage_sources->add_damage(Damage_source::deep_wounds, buff.damage, current_time);
                if (debug)
                {
                    debug_msg.emplace_back("Over time effect: " + buff.name + " tick. Damage: " + std::to_string(int(buff.damage)));
                }
            }
            else
            {
                (*simulation_special_stats) += buff.special_stats;
            }

            if (buff.next_fade < current_time)
            {
                buff.next_tick = std::numeric_limits<double>::max();
                if (debug)
                {
                    debug_msg.emplace_back("Over time effect: " + buff.name + " fades.");
                }
            }
            else
            {
                buff.next_tick += buff.interval;
            }

            min_over_time_buff = std::min(min_over_time_buff, buff.next_tick);
        }
    }

    void reset_icd(const Hit_effect& hit_effect, double current_time) const
    {
        if (hit_effect.cooldown == 0) return;

        for (auto& hit_effect_mh : *hit_effects_mh)
        {
            if (hit_effect_mh.name == hit_effect.name)
            {
                hit_effect_mh.time_counter = current_time + hit_effect.cooldown;
                break;
            }
        }
        for (auto& hit_effect_oh : *hit_effects_oh)
        {
            if (hit_effect_oh.name == hit_effect.name)
            {
                hit_effect_oh.time_counter = current_time + hit_effect.cooldown;
                break;
            }
        }
    }

    /*
    // TODO this can most likely be removed - special case for something that's not very special
    void modify_arpen_stacks(Hit_effect& hit_effect, Socket weapon_hand, bool at_max_stacks = false)
    {
        Special_stats no_reduction = {};
        Special_stats armor_reduction = {};
        armor_reduction.gear_armor_pen = hit_effect.armor_reduction;

        need_to_recompute_mitigation = true;
        if (weapon_hand == Socket::main_hand)
        {
            for (auto& hit_effect_mh : *hit_effects_mh)
            {
                if (hit_effect_mh.name == hit_effect.name)
                {
                    add(hit_effect_mh.name, at_max_stacks ? no_reduction : armor_reduction, hit_effect_mh.duration, false, true);
                    break;
                }
            }
        }
        else
        {
            for (auto& hit_effect_oh : *hit_effects_oh)
            {
                if (hit_effect_oh.name == hit_effect.name)
                {
                    armor_reduction.gear_armor_pen = hit_effect.armor_reduction;
                    add(hit_effect_oh.name, at_max_stacks ? no_reduction : armor_reduction, hit_effect_oh.duration, false, true);
                    break;
                }
            }
        }
    }
    */

    void increment(double dt, double current_time, double& rage, double& rage_lost_stance, Time_keeper& time_keeper,
                   bool debug, std::vector<std::string>& debug_msg, double ap_multiplier)
    {
        increment_combat_buffs(current_time, rage, rage_lost_stance, debug, debug_msg);
        if (false) increment_hit_gains(current_time, dt, debug, debug_msg);
        // increment_hit_stacks(current_time, dt, debug, debug_msg);
        increment_use_effects(current_time, rage, time_keeper, debug, debug_msg, ap_multiplier);
        increment_over_time_buffs(current_time, rage, debug, debug_msg);
    }

    // single - extend duration on refresh
    // stack - extend duration on refresh
    void gain_stats(const Special_stats& ssb)
    {
        (*simulation_special_stats) += ssb;
        need_to_recompute_hit_tables |= (ssb.hit > 0 || ssb.critical_strike > 0 || ssb.expertise > 0);
        need_to_recompute_mitigation |= (ssb.gear_armor_pen > 0);
    }

    //void add(const std::string& name, const Special_stats& special_stats_boost, double duration, double current_time)
    void update_min_combat_buff(const Combat_buff& buff, double current_time)
    {
        if (buff.next_fade >= current_time && buff.next_fade < min_combat_buff)
        {
            min_combat_buff = buff.next_fade;
        }
    }

    // Hit_effect's name is used as a Combat_buff name here, so a "stat boost" hit effect
    //  has one, and only one corresponding buff.
    // This allows to setup a hit_effect -> buff connection initially, which allows to cut
    //  short buff iteration.
    // however, min_combat_buff might not be updated correctly, so increment_combat_buff()
    //  does more work
    // if stacks work correctly (i.e. stacks == 0 || stacks < max_stacks determine whether
    //  it's a gain or a refresh), next_fade could be set to MAX_DOUBLE on buff fade

    void add_combat_buff(const Hit_effect& hit_effect, double current_time)
    {
        assert(hit_effect.max_stacks >= 1);

        // getting here, min_combat_buff is set correctly: if no buff is active, it's at "max()", otherwise it's >= current_time.
        //  the gained or refreshed buff is also >= current_time, so it competes for the new min_combat_buff;
        //  all other buffs must be ignored
        auto refresh = false;
        min_combat_buff = std::numeric_limits<double>::max();
        for (auto& buff : combat_buffs)
        {
            if (buff.name == hit_effect.name)
            {
                if (buff.next_fade < current_time || buff.stacks < hit_effect.max_stacks)
                {
                    if (buff.next_fade < current_time) assert(buff.stacks == 0);
                    if (buff.stacks == 0) buff.last_gain = current_time;
                    gain_stats(buff.special_stats_boost);
                    buff.stacks += 1;
                }
                buff.next_fade = current_time + hit_effect.duration; // or keep unchanged for "temporary hit effects"
                refresh = true;
            }
            update_min_combat_buff(buff, current_time);
        }
        if (!refresh)
        {
            auto& buff = combat_buffs.emplace_back(Combat_buff(hit_effect, *simulation_special_stats, current_time));
            gain_stats(buff.special_stats_boost);
            min_combat_buff = std::min(min_combat_buff, buff.next_fade);
        }
    }

    void add_hit_effect(const std::string& name, const Hit_effect& hit_effect, double duration_left)
    {
        exit(55);
        (*hit_effects_mh).emplace_back(hit_effect);
        (*hit_effects_oh).emplace_back(hit_effect);
        hit_gains.emplace_back(name, duration_left);
    }

    void update_min_over_time_buff(const Over_time_buff& buff, double current_time)
    {
        if (buff.next_tick >= current_time && buff.next_tick < min_over_time_buff)
        {
            min_over_time_buff = buff.next_tick;
        }
    }

    void add_over_time_buff(const Over_time_effect& over_time_effect, double current_time)
    {
        auto refresh = false;
        min_over_time_buff = std::numeric_limits<double>::max();
        for (auto& buff : over_time_buffs)
        {
            if (buff.name == over_time_effect.name)
            {
                buff.damage = over_time_effect.damage;
                //buff.rage_gain = over_time_effect.rage_gain;
                //buff.special_stats = over_time_effect.special_stats;
                //buff.interval = over_time_effect.interval;
                buff.next_tick = current_time + over_time_effect.interval;
                buff.next_fade = current_time + over_time_effect.duration;
                refresh = true;
            }
            update_min_over_time_buff(buff, current_time);
        }
        if (!refresh)
        {
            auto& buff = over_time_buffs.emplace_back(Over_time_buff(over_time_effect, current_time));
            min_over_time_buff = std::min(min_over_time_buff, buff.next_tick);
        }
    }

    bool need_to_recompute_hit_tables{false};
    bool need_to_recompute_mitigation{false};
    Special_stats* simulation_special_stats;
    Damage_sources* simulation_damage_sources;

    std::vector<Combat_buff> combat_buffs;
    double min_combat_buff = std::numeric_limits<double>::max();

    std::vector<Over_time_buff> over_time_buffs;
    double min_over_time_buff = std::numeric_limits<double>::max();

    std::vector<Hit_buff> hit_gains;
    std::vector<Hit_effect>* hit_effects_mh;
    std::vector<Hit_effect>* hit_effects_oh;
    std::vector<std::pair<double, Use_effect>> use_effect_order;
    std::unordered_map<std::string, double> aura_uptime;
    double tactical_mastery_rage_{};
};

#endif // WOW_SIMULATOR_BUFF_MANAGER_HPP
