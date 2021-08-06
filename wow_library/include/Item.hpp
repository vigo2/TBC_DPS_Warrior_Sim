#ifndef WOW_SIMULATOR_ITEM_HPP
#define WOW_SIMULATOR_ITEM_HPP

#include "Attributes.hpp"

#include <cassert>
#include <utility>

enum class Socket
{
    none,
    head,
    neck,
    shoulder,
    back,
    chest,
    wrist,
    hands,
    belt,
    legs,
    boots,
    ring,
    trinket,
    main_hand,
    off_hand,
    ranged,
};

enum class Weapon_socket
{
    main_hand,
    one_hand,
    off_hand,
    two_hand
};

enum class Weapon_type
{
    sword,
    axe,
    dagger,
    mace,
    unarmed
};

enum class Set
{
    none,
    ragesteel,
    wastewalker,
    doomplate,
    warbringer,

};

/* enum class Gem_bonus
{
    none
};


struct Gem
{
    enum class Type
    {
    strength3, 
    strength5,
    strength4, 
    strength6, 
    strength8,
    agility3, 
    agility4, 
    agility5,
    agility6, 
    agility10, 
    AP20, 
    AP24,
    crit3,  
    crit4,
    crit5, 
    crit6, 
    crit8, 
    crit10, 
    crit12, 
    hit4, 
    hit6, 
    hit8, 
    hit10, 
    hit12,
    dmg3,  
    agi12_critDmg3, 
    haste_proc,
    crit3_str3, 
    crit4_str4, 
    crit4_str5, 
    crit5_str5,
    empty,
    none
    };

    Gem() = default;

    explicit Gem(Type type) : type(type){};

    Type type{};
    Attributes attributes{};
    Special_stats special_stats{};
}; */

struct Over_time_effect
{
    Over_time_effect() = default;

    ~Over_time_effect() = default;

    Over_time_effect(std::string name, Special_stats special_stats, double rage_gain, double damage, int interval,
                     int duration)
        : name(std::move(name))
        , special_stats(special_stats)
        , rage_gain(rage_gain)
        , damage(damage)
        , interval(interval)
        , duration(duration) {};

    std::string name;
    Special_stats special_stats;
    double rage_gain;
    double damage;
    int interval;
    int duration;

    int over_time_buff_idx{-1};
};

class Hit_effect
{
public:
    enum class Type
    {
        none,
        extra_hit,
        windfury_hit,
        sword_spec,
        stat_boost,
        damage_physical,
        damage_magic,
        reduce_armor
    };

    Hit_effect() = default;

    Hit_effect(std::string name, Type type, Attributes attribute_boost, Special_stats special_stats_boost,
               double damage, double duration, double cooldown, double probability, double attack_power_boost = 0, int n_targets = 1,
               int armor_reduction = 0, int max_stacks = 1, double ppm = 0.0, bool affects_both_weapons = false)
        : name(std::move(name))
        , type(type)
        , attribute_boost(attribute_boost)
        , special_stats_boost(special_stats_boost)
        , damage(damage)
        , duration(duration)
        , cooldown(cooldown)
        , probability(probability)
        , attack_power_boost(attack_power_boost)
        , n_targets(n_targets)
        , armor_reduction(armor_reduction)
        , ppm(ppm)
        , affects_both_weapons(affects_both_weapons)
        , max_stacks(max_stacks)
        , time_counter(0)
        , procs(0)
        , combat_buff_idx(-1) {};

    void sanitize()
    {
        //if (probability == 0)
        //{
        //    probability = e.ppm * weapon.swing_speed / 60;
        //}

        if (type == Hit_effect::Type::reduce_armor)
        {
            type = Hit_effect::Type::stat_boost;
            special_stats_boost.gear_armor_pen = armor_reduction;
        }

        if (max_stacks == 0)
        {
            max_stacks = 1;
        }
    }

    // FIXME(vigo) ap_multiplier is used for attributes, but not for special_stats...
    [[nodiscard]] Special_stats get_special_stat_equivalent(const Special_stats& special_stats, double ap_multiplier = 0) const
    {
        return attribute_boost.convert_to_special_stats(special_stats, ap_multiplier) + special_stats_boost;
    }

    std::string name;
    Type type;
    Attributes attribute_boost;
    Special_stats special_stats_boost;
    double damage;
    double duration;
    double cooldown;
    double probability; // derived for some
    double attack_power_boost; // should be const, once windfury is fixed (only usage?)
    int n_targets;
    int armor_reduction;
    double ppm;
    bool affects_both_weapons;
    int max_stacks{1}; // should be const, once "fixed" to one

    double time_counter; // "next_ready", aka cooldown end

    int procs; // statistics

    int combat_buff_idx{-1}; // organizational ;)
};

class Use_effect
{
public:
    enum class Effect_socket
    {
        shared,
        unique,
    };

    Use_effect() = default;

    ~Use_effect() = default;

    Use_effect(std::string name, Effect_socket effect_socket, Attributes attribute_boost,
               Special_stats special_stats_boost, double rage_boost, double duration, double cooldown,
               bool triggers_gcd, std::vector<Hit_effect> hit_effects = std::vector<Hit_effect>(),
               std::vector<Over_time_effect> over_time_effects = std::vector<Over_time_effect>())
        : name(std::move(name))
        , effect_socket(effect_socket)
        , attribute_boost(attribute_boost)
        , special_stats_boost(special_stats_boost)
        , rage_boost(rage_boost)
        , duration(duration)
        , cooldown(cooldown)
        , triggers_gcd(triggers_gcd)
        , hit_effects(std::move(hit_effects))
        , over_time_effects(std::move(over_time_effects)){};

    // FIXME(vigo) ap_multiplier is used for attributes, but not for special_stats...
    inline Special_stats get_special_stat_equivalent(const Special_stats& special_stats, double ap_multiplier = 0) const
    {
        return attribute_boost.convert_to_special_stats(special_stats, ap_multiplier) + special_stats_boost;
    }

    std::string name;
    Effect_socket effect_socket{};
    Attributes attribute_boost{};
    Special_stats special_stats_boost{};
    double rage_boost{};
    double duration{};
    double cooldown{};
    bool triggers_gcd{false};
    std::vector<Hit_effect> hit_effects{};
    std::vector<Over_time_effect> over_time_effects{};
};

struct Enchant
{
    enum class Type
    {
        none,
        strength,
        strength7,
        strength9,
        strength12,
        strength15,
        strength20,
        agility,
        agility12,
        greater_agility,
        haste,
        crusader,
        minor_stats,
        major_stats,
        ring_stats,
        ring_damage,
        attack_power,
        naxxramas,
        damage,
        ferocity,
        greater_vengeance,
        greater_blade,
        exceptional_stats,
        cobrahide,
        nethercobra,
        mongoose,
        hit,
        cats_swiftness
    };

    Enchant() = default;

    explicit Enchant(Type type) : type(type){};

    Type type{};
    Attributes attributes{};
    Special_stats special_stats{};
};

struct Set_bonus
{
    Set_bonus(std::string name, Attributes attributes, Special_stats special_stats, int pieces, Set set)
        : name(std::move(name)), attributes(attributes), special_stats(special_stats), pieces(pieces), set(set){};

    std::string name;
    Attributes attributes;
    Special_stats special_stats;
    int pieces;
    Set set;
};

struct Buff
{
    Buff(std::string name, Attributes attributes, Special_stats special_stats, double bonus_damage = 0,
         std::vector<Hit_effect> hit_effects = std::vector<Hit_effect>(),
         std::vector<Use_effect> use_effects = std::vector<Use_effect>())
        : name(std::move(name))
        , attributes(attributes)
        , special_stats(special_stats)
        , bonus_damage(bonus_damage)
        , hit_effects(std::move(hit_effects))
        , use_effects(std::move(use_effects)){};

    std::string name;
    Attributes attributes;
    Special_stats special_stats;
    double bonus_damage;
    std::vector<Hit_effect> hit_effects{};
    std::vector<Use_effect> use_effects{};
};

// struct Gem
// {
//     Gem(std::string name, Attributes attributes, Special_stats special_stats, double bonus_damage = 0,
//          std::vector<Hit_effect> hit_effects = std::vector<Hit_effect>(),
//          std::vector<Use_effect> use_effects = std::vector<Use_effect>())
//         : name(std::move(name))
//         , attributes(attributes)
//         , special_stats(special_stats)
//         , bonus_damage(bonus_damage)
//         , hit_effects(std::move(hit_effects))
//         , use_effects(std::move(use_effects)){};

//     std::string name;
//     Attributes attributes;
//     Special_stats special_stats;
//     double bonus_damage;
//     std::vector<Hit_effect> hit_effects{};
//     std::vector<Use_effect> use_effects{};
// };

struct Weapon_buff
{
    Weapon_buff() = default;

    Weapon_buff(std::string name, Attributes attributes, Special_stats special_stats, double bonus_damage = 0)
        : name(std::move(name)), attributes(attributes), special_stats(special_stats), bonus_damage(bonus_damage){};

    std::string name{};
    Attributes attributes{};
    Special_stats special_stats{};
    double bonus_damage{};
};

struct Armor
{
    Armor(std::string name, Attributes attributes, Special_stats special_stats, Socket socket, Set set_name = Set::none,
          std::vector<Hit_effect> hit_effects = std::vector<Hit_effect>(),
          std::vector<Use_effect> use_effects = std::vector<Use_effect>())
        : name(std::move(name))
        , attributes(attributes)
        , special_stats(special_stats)
        , socket(socket)
        , set_name(set_name)
        , hit_effects(std::move(hit_effects))
        , use_effects(std::move(use_effects)){};
    std::string name;
    Attributes attributes;
    Special_stats special_stats;
    Socket socket;
    Set set_name;
    Enchant enchant{};
    std::vector<Hit_effect> hit_effects{};
    std::vector<Use_effect> use_effects{};
};

struct Weapon
{
    Weapon(std::string name, Attributes attributes, Special_stats special_stats, double swing_speed, double min_damage,
           double max_damage, Weapon_socket weapon_socket, Weapon_type weapon_type,
           std::vector<Hit_effect> hit_effects = std::vector<Hit_effect>(), Set set_name = Set::none,
           std::vector<Use_effect> use_effects = std::vector<Use_effect>())
        : name(std::move(name))
        , attributes(attributes)
        , special_stats(special_stats)
        , swing_speed(swing_speed)
        , min_damage(min_damage)
        , max_damage(max_damage)
        , weapon_socket(weapon_socket)
        , type(weapon_type)
        , hit_effects(std::move(hit_effects))
        , set_name(set_name)
        , use_effects(std::move(use_effects)){};

    std::string name;
    Attributes attributes;
    Special_stats special_stats;
    double swing_speed;
    double min_damage;
    double max_damage;
    Weapon_socket weapon_socket;
    Weapon_type type;
    std::vector<Hit_effect> hit_effects;
    Set set_name;
    std::vector<Use_effect> use_effects;
    Socket socket;
    Enchant enchant;
    Weapon_buff buff;
};

std::ostream& operator<<(std::ostream& os, const Socket& socket);

std::string operator+(std::string& string, const Socket& socket);

std::string operator+(std::string& string, const Weapon_socket& socket);

#endif // WOW_SIMULATOR_ITEM_HPP
