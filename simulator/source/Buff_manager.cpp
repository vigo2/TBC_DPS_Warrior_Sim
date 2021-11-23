#include "Buff_manager.hpp"

#include "Item.hpp" // should be redundant

void Buff_manager::initialize(std::vector<Hit_effect>& hit_effects_mh_input, std::vector<Hit_effect>& hit_effects_oh_input,
                Use_effects::Schedule& use_effects_schedule_input, Rage_manager* rage_manager_input)
{
    hit_effects_mh = &hit_effects_mh_input;
    hit_effects_oh = &hit_effects_oh_input;

    use_effects_schedule = use_effects_schedule_input;

    rage_manager = rage_manager_input;
}

void Buff_manager::reset(Sim_state& state)
{
    sim_state = &state;

    for (auto& he : *hit_effects_mh)
    {
        he.time_counter = 0;
    }

    for (auto& he : *hit_effects_oh)
    {
        he.time_counter = 0;
    }

    for (auto& buff : combat_buffs)
    {
        buff.stacks = 0;
        buff.next_fade = std::numeric_limits<int>::max();
    }
    min_combat_buff = std::numeric_limits<int>::max();

    for (auto& buff : over_time_buffs)
    {
        buff.next_tick = Over_time_buff::inactive;
    }
    min_over_time_buff = std::numeric_limits<int>::max();

    for (auto& hit_aura : hit_auras)
    {
        // update hit effect pointers, since hit_effects are permutated now
        auto mh = std::find_if(hit_effects_mh->begin(), hit_effects_mh->end(), [&hit_aura](auto& he) { return he.name == hit_aura.name; });
        assert(mh != hit_effects_mh->end());
        hit_aura.hit_effect_mh = &(*mh);

        auto oh = std::find_if(hit_effects_oh->begin(), hit_effects_oh->end(), [&hit_aura](auto& he) { return he.name == hit_aura.name; });
        assert(oh != hit_effects_oh->end());
        hit_aura.hit_effect_oh = &(*oh);

        hit_aura.next_fade = Hit_aura::inactive;
        hit_aura.hit_effect_mh->time_counter = std::numeric_limits<int>::max();
        hit_aura.hit_effect_oh->time_counter = std::numeric_limits<int>::max();
    }
    min_hit_aura = std::numeric_limits<int>::max();

    use_effect_index = 0;
    min_use_effect = use_effects_schedule.empty() ? std::numeric_limits<int>::max() : use_effects_schedule[0].first - 1;

    need_to_recompute_mitigation = true;
    need_to_recompute_hit_tables = true;
}

void Buff_manager::update_aura_uptimes(int current_time) {
    for (auto& buff : combat_buffs)
    {
        for (int i = 0; i < buff.stacks; ++i) {
            buff.per_stack_uptime[i] += current_time - (buff.per_stack_last_gain[i] > 0 ? buff.per_stack_last_gain[i] : 0);
        }
    }
    for (auto& buff : over_time_buffs)
    {
        if (buff.next_tick != Over_time_buff::inactive) buff.uptime += current_time - (buff.last_gain > 0 ? buff.last_gain : 0);
    }
}

[[nodiscard]] std::unordered_map<std::string, double> Buff_manager::get_aura_uptimes_map() const
{
    auto m = std::unordered_map<std::string, double>();
    for (const auto& buff : combat_buffs)
    {
        m[buff.name] = static_cast<double>(buff.per_stack_uptime[0]) * 0.001;
        for (int i = 1; i < buff.max_stacks; ++i) {
            m[buff.name + " (" + std::to_string(i + 1) + ")"] = static_cast<double>(buff.per_stack_uptime[i]) * 0.001;
        }
    }
    for (const auto& buff : over_time_buffs)
    {
        m[buff.name] = static_cast<double>(buff.uptime) * 0.001;
    }
    return m;
}

void Buff_manager::increment(Time_keeper& time_keeper, Logger& logger)
{
    auto current_time = time_keeper.time;
    increment_combat_buffs(current_time, logger);
    increment_over_time_buffs(current_time, logger);
    increment_hit_auras(current_time, logger);
    increment_use_effects(current_time, time_keeper, logger);
}

void Buff_manager::remove_charge(const Hit_effect& hit_effect, int current_time, Logger& logger)
{
    if (hit_effect.combat_buff_idx == -1)
    {
        // it's required that hit_effect can be successfully registered (i.e. combat_buff_idx is eventually set)
        return; // not up
    }

    auto& buff = combat_buffs[hit_effect.combat_buff_idx];
    if (buff.stacks == 0) return;
    buff.charges -= 1;
    if (buff.charges > 0) return;
    buff.next_fade = current_time; // for correct uptime bookkeeping
    do_fade_buff(buff, logger);
}

// instead of using hit_effects per weapon, and "sharing" them via combat_buff name,
//  hit_effects could also be registered "globally" (together with their respective combat_buff,
//  so each hit_effect would have a one-to-one connection to the corresponding buff),
//  and would simply apply with different percentages
void Buff_manager::add_combat_buff(Hit_effect& hit_effect, int current_time)
{
    assert(hit_effect.max_stacks >= 1);
    assert(hit_effect.max_charges >= 1);

    // "registration", essentially - once per hit_effect, connects each hit_effect w/ a combat buff
    if (hit_effect.combat_buff_idx == -1)
    {
        for (size_t i = 0; i < combat_buffs.size(); ++i)
        {
            if (combat_buffs[i].name == hit_effect.name)
            {
                hit_effect.combat_buff_idx = static_cast<int>(i);
                return do_add_combat_buff(hit_effect, current_time);
            }
        }

        auto& buff = combat_buffs.emplace_back(hit_effect, sim_state->special_stats, current_time);
        hit_effect.combat_buff_idx = static_cast<int>(combat_buffs.size()) - 1;
        gain_stats(buff.special_stats_boost);
        if (buff.next_fade < min_combat_buff) min_combat_buff = buff.next_fade;
        return;
    }

    do_add_combat_buff(hit_effect, current_time);
}

void Buff_manager::add_hit_aura(const std::string& name, Hit_effect& hit_effect, int duration, int current_time)
{
    for (auto& hit_aura : hit_auras)
    {
        if (hit_aura.name == name)
        {
            hit_aura.hit_effect_mh->time_counter = 0; // re-enable hit_effects, and queue fade
            hit_aura.hit_effect_oh->time_counter = 0;
            hit_aura.next_fade = current_time + duration;
            if (hit_aura.next_fade < min_hit_aura) min_hit_aura = hit_aura.next_fade;
            return;
        }
    }

    auto& hit_aura = hit_auras.emplace_back(Hit_aura(name, current_time, duration));
    hit_effect.sanitize();
    hit_aura.hit_effect_mh = &hit_effects_mh->emplace_back(hit_effect);
    hit_aura.hit_effect_oh = &hit_effects_oh->emplace_back(hit_effect);
    if (hit_aura.next_fade < min_hit_aura) min_hit_aura = hit_aura.next_fade;
}

void Buff_manager::add_over_time_buff(Over_time_effect& over_time_effect, int current_time)
{
    if (over_time_effect.over_time_buff_idx == -1)
    {
        for (size_t i = 0; i < over_time_buffs.size(); ++i)
        {
            if (over_time_buffs[i].name == over_time_effect.name)
            {
                over_time_effect.over_time_buff_idx = static_cast<int>(i);
                return do_add_over_time_buff(over_time_effect, current_time);
            }
        }

        auto& buff = over_time_buffs.emplace_back(Over_time_buff(over_time_effect, current_time));
        if (buff.next_tick < min_over_time_buff) min_over_time_buff = buff.next_tick;
        over_time_effect.over_time_buff_idx = static_cast<int>(over_time_buffs.size()) - 1;
        return;
    }

    do_add_over_time_buff(over_time_effect, current_time);
}


void Buff_manager::increment_combat_buffs(int current_time, Logger& logger)
{
    if (current_time < min_combat_buff)
    {
        return;
    }

    min_combat_buff = std::numeric_limits<int>::max();
    for (auto& buff : combat_buffs)
    {
        if (buff.stacks == 0) // inactive
        {
            continue;
        }

        if (buff.next_fade > current_time) // not ready
        {
            if (buff.next_fade < min_combat_buff) min_combat_buff = buff.next_fade;
            continue;
        }

        assert(current_time == buff.next_fade);

        do_fade_buff(buff, logger);
    }
}

void Buff_manager::increment_over_time_buffs(int current_time, Logger& logger)
{
    if (current_time < min_over_time_buff)
    {
        return;
    }

    min_over_time_buff = std::numeric_limits<int>::max();
    for (auto& buff : over_time_buffs)
    {
        if (buff.next_tick == Over_time_buff::inactive) // inactive
        {
            continue;
        }

        if (buff.next_tick > current_time) // not ready
        {
            if (buff.next_tick < min_over_time_buff) min_over_time_buff = buff.next_tick;
            continue;
        }

        // if over_time_buffs start pre-combat (bloodrage), next_tick might be < 0, and can't be scheduled correctly
        assert(current_time == (buff.next_tick > 0 ? buff.next_tick : 0));

        // this used to support everything at once, but no over_time_buff actually granted rage, dealt damage, and added stats.
        //  nothing does the latter, afaik
        if (buff.rage_gain > 0)
        {
            rage_manager->gain_rage(buff.rage_gain);
            logger.print("Over time effect: ", buff.name, " tick. Current rage: ", int(rage_manager->get_rage()));
        }
        else if (buff.damage > 0)
        {
            sim_state->add_damage(Damage_source::deep_wounds, buff.damage, current_time);
            logger.print("Over time effect: ", buff.name, " tick. Damage: ", int(buff.damage));
        }
        else
        {
            sim_state->special_stats += buff.special_stats;
        }

        if (buff.next_fade == current_time)
        {
            buff.next_tick = Over_time_buff::inactive;
            buff.uptime += buff.next_fade - (buff.last_gain > 0 ? buff.last_gain : 0);
            logger.print("Over time effect: ", buff.name, " fades.");
        }
        else
        {
            buff.next_tick += buff.interval;
            if (buff.next_tick < min_over_time_buff) min_over_time_buff = buff.next_tick;
        }
    }
}

void Buff_manager::increment_hit_auras(int current_time, Logger& logger)
{
    if (current_time < min_hit_aura)
    {
        return;
    }

    min_hit_aura = std::numeric_limits<int>::max();
    for (auto& hit_aura : hit_auras)
    {
        if (hit_aura.next_fade == Hit_aura::inactive)
        {
            continue;
        }

        if (hit_aura.next_fade > current_time)
        {
            if (hit_aura.next_fade < min_hit_aura) min_hit_aura = hit_aura.next_fade;
            continue;
        }

        assert(current_time == hit_aura.next_fade);

        assert(hit_aura.hit_effect_mh != nullptr);
        assert(hit_aura.hit_effect_oh != nullptr);

        // or have a specialized add_combat_buff() here, probably
        assert(hit_aura.hit_effect_mh->combat_buff_idx >= 0);
        assert(hit_aura.hit_effect_oh->combat_buff_idx == -1 || hit_aura.hit_effect_oh->combat_buff_idx == hit_aura.hit_effect_mh->combat_buff_idx);

        hit_aura.hit_effect_mh->time_counter = std::numeric_limits<int>::max(); // effectively disable hit_effects
        hit_aura.hit_effect_oh->time_counter = std::numeric_limits<int>::max();

        auto& buff = combat_buffs[hit_aura.hit_effect_mh->combat_buff_idx];
        buff.next_fade = hit_aura.next_fade; // for correct uptime bookkeeping
        do_fade_buff(buff, logger);

        hit_aura.next_fade = Hit_aura::inactive;
    }
}

void Buff_manager::increment_use_effects(int current_time, Time_keeper& time_keeper, Logger& logger)
{
    if (current_time < min_use_effect)
    {
        return;
    }

    auto& use_effect = use_effects_schedule[use_effect_index].second.get();

    if (use_effect.triggers_gcd && !time_keeper.global_ready())
    {
        min_use_effect = current_time + time_keeper.global_cd();
        return;
    }

    if (use_effect.rage_boost > 0 && rage_manager->get_rage() + use_effect.rage_boost > 100)
    {
        min_use_effect = current_time + 500;
        return;
    }

    if (use_effect.rage_boost < 0 && rage_manager->get_rage() + use_effect.rage_boost < 0)
    {
        min_use_effect = current_time + 500;
        return;
    }

    if (!use_effect.hit_effects.empty())
    {
        add_hit_aura(use_effect.name, use_effect.hit_effects[0], use_effect.hit_effects[0].duration, current_time);
    }
    else if (!use_effect.over_time_effects.empty())
    {
        add_over_time_buff(use_effect.over_time_effects[0], current_time);
    }
    else
    {
        add_combat_buff(use_effect.combat_buff, current_time);
    }

    if (use_effect.rage_boost > 0)
    {
        rage_manager->gain_rage(use_effect.rage_boost);
        logger.print("Current rage: ", int(rage_manager->get_rage()));
    }

    if (use_effect.rage_boost < 0)
    {
        rage_manager->spend_rage(-use_effect.rage_boost);
        logger.print("Current rage: ", int(rage_manager->get_rage()));
    }

    if (use_effect.triggers_gcd)
    {
        time_keeper.global_cast(1500);
    }

    use_effect_index += 1;

    if (use_effect_index < use_effects_schedule.size())
    {
        min_use_effect = use_effects_schedule[use_effect_index].first;

        const auto& ue = use_effects_schedule[use_effect_index].second.get();
        if (ue.triggers_gcd || ue.rage_boost != 0) min_use_effect -= 1000;
    }
    else
    {
        min_use_effect = std::numeric_limits<int>::max();
    }
}


void Buff_manager::do_fade_buff(Combat_buff& buff, Logger& logger)
{
    const auto& ssb = buff.special_stats_boost;
    for (int i = 0; i < buff.stacks; i++)
    {
        sim_state->special_stats -= ssb;
    }

    for (auto i = 0; i < buff.stacks; ++i) {
        buff.per_stack_uptime[i] += buff.next_fade - (buff.per_stack_last_gain[i] > 0 ? buff.per_stack_last_gain[i] : 0);
    }

    buff.stacks = 0;
    buff.charges = 0;

    need_to_recompute_hit_tables |= (ssb.critical_strike > 0 || ssb.hit > 0 || ssb.expertise > 0);
    need_to_recompute_mitigation |= (ssb.gear_armor_pen > 0);

    // special case, should be removed
    if (buff.name == "battle_stance")
    {
        rage_manager->swap_stance();
    }

    logger.print(buff.name, " fades.");
}

void Buff_manager::gain_stats(const Special_stats& ssb)
{
    sim_state->special_stats += ssb;
    need_to_recompute_hit_tables |= (ssb.hit > 0 || ssb.critical_strike > 0 || ssb.expertise > 0);
    need_to_recompute_mitigation |= (ssb.gear_armor_pen > 0);
}

void Buff_manager::do_add_combat_buff(Hit_effect& hit_effect, int current_time)
{
    auto& buff = combat_buffs[hit_effect.combat_buff_idx];
    if (buff.next_fade < current_time || buff.stacks < hit_effect.max_stacks)
    {
        if (buff.next_fade < current_time) assert(buff.stacks == 0 && buff.charges == 0);
        gain_stats(buff.special_stats_boost);
        buff.per_stack_last_gain[buff.stacks] = current_time;
        buff.stacks += 1;
    }
    buff.next_fade = current_time + hit_effect.duration; // or keep unchanged for "temporary hit effects"
    buff.charges = hit_effect.max_charges;
    if (buff.next_fade < min_combat_buff) min_combat_buff = buff.next_fade;
}

void Buff_manager::do_add_over_time_buff(const Over_time_effect& over_time_effect, int current_time)
{
    auto& buff = over_time_buffs[over_time_effect.over_time_buff_idx];

    if (buff.next_tick == Over_time_buff::inactive) {
        buff.last_gain = current_time;
    }

    buff.damage = over_time_effect.damage; // nothing else ever changes

    buff.next_tick = current_time + over_time_effect.interval;
    buff.next_fade = current_time + over_time_effect.duration;
    if (buff.next_tick < min_over_time_buff) min_over_time_buff = buff.next_tick;
}
