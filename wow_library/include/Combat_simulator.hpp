#ifndef WOW_SIMULATOR_COMBAT_SIMULATOR_HPP
#define WOW_SIMULATOR_COMBAT_SIMULATOR_HPP

#include <vector>
#include <iostream>
#include <cassert>
#include <map>
#include <cmath>
#include <iomanip>
#include <random>

#include "Character.hpp"
#include "damage_sources.hpp"
#include "time_keeper.hpp"
#include "weapon_sim.hpp"
#include "Buff_manager.hpp"
#include "Statistics.hpp"

struct Combat_simulator_config
{
    // Combat settings
    int n_batches{};
    double sim_time{};
    int opponent_level{};

    bool curse_of_recklessness_active{false};
    bool sunder_armor_active{false};
    bool faerie_fire_feral_active{false};
    int n_sunder_armor_stacks = 0;

    // Simulator settings
    bool use_sim_time_ramp = false;
    bool enable_spell_rotation{false};
    bool use_mighty_rage_potion{false};
    bool enable_bloodrage{false};
    bool enable_recklessness{false};
    bool display_combat_debug{false};
    bool use_seed{false};
    int seed{};
    bool fuel_extra_rage{false};
    int extra_rage_interval{};
    int extra_rage_damage_amount{};

    struct talents_t
    {
        bool death_wish{false};
        bool anger_management{false};
        int improved_heroic_strike = 0;
        int flurry = 0;
        int unbridled_wrath = 0;
        int impale = 0;
        int improved_execute = 0;
        int dual_wield_specialization = 0;
    } talents;
};

class Combat_simulator
{
public:
    explicit Combat_simulator(Combat_simulator_config config) : config(config)
    {
        if (config.use_seed)
        {
            srand(config.seed);
        }
    }

    enum class Hit_result
    {
        miss,
        dodge,
        glancing,
        crit,
        hit,
        TBD
    };

    enum class Hit_type
    {
        white,
        yellow
    };

    struct Hit_outcome
    {
        Hit_outcome(double damage, Hit_result hit_result) : damage{damage}, hit_result{hit_result} {};

        double damage;
        Hit_result hit_result;
    };

    constexpr double lookup_outcome_mh(int case_id)
    {
        switch (case_id)
        {
            case 0:
                return 0.0;
            case 1:
                return 0.0;
            case 2:
                return glancing_factor_mh_;
            case 3:
                return 2.0 + 0.1 * config.talents.impale;
            case 4:
                return 1.0;
            default:
                assert(false);
                return 0.0;
        }
    }

    constexpr double lookup_outcome_oh(int case_id)
    {
        switch (case_id)
        {
            case 0:
                return 0.0;
            case 1:
                return 0.0;
            case 2:
                return glancing_factor_oh_;
            case 3:
                return 2.0 + 0.1 * config.talents.impale;
            case 4:
                return 1.0;
            default:
                assert(false);
                return 0.0;
        }
    }

    void
    manage_flurry(Hit_result hit_result, Special_stats &special_stats, int &flurry_charges, bool is_ability = false);

    void swing_weapon(Weapon_sim &weapon, Weapon_sim &main_hand_weapon, Special_stats &special_stats,
                      bool &heroic_strike_active, double &rage, double &heroic_strike_rage_cost,
                      bool &recklessness_active, Damage_sources &damage_sources, int &flurry_charges);

    std::vector<double> &simulate(const Character &character, int n_batches);

    std::vector<double> &simulate(const Character &character);

    static double get_uniform_random(double r_max)
    {
        return rand() * r_max / RAND_MAX;
    }

    Combat_simulator::Hit_outcome
    generate_hit(double damage, Hit_type hit_type, Socket weapon_hand, bool heroic_strike_active,
                 const Special_stats &special_stats, bool recklessness_active);

    Combat_simulator::Hit_outcome generate_hit_oh(double damage, bool heroic_strike_active, bool recklessness_active);

    Combat_simulator::Hit_outcome generate_hit_mh(double damage, Hit_type hit_type, bool recklessness_active);

    void compute_hit_table(int level_difference, int weapon_skill, Special_stats special_stats, Socket weapon_hand);

    const std::vector<double> &get_hit_probabilities_white_mh() const;

    void cout_damage_parse(Combat_simulator::Hit_type hit_type, Socket weapon_hand,
                           Combat_simulator::Hit_outcome hit_outcome);

    void print_damage_distribution() const;

    std::vector<Damage_sources> get_damage_distribution() const;

    std::vector<std::string> get_aura_uptimes() const;

    template<typename T>
    void print_statement(T t)
    {
        std::cout << std::setprecision(4) << t;
    }

    template<typename... Args>
    void simulator_cout(Args &&... args)
    {
        if (config.display_combat_debug)
        {
            std::cout << "Time: " << std::setw(8) << std::left << time_keeper_.time
                      << "s. Loop idx:" << std::setw(4) << time_keeper_.step_index << "Event: ";
            __attribute__((unused)) int dummy[] = {0, ((void) print_statement(std::forward<Args>(args)), 0)...};
            std::cout << "\n";
        }
    }

    Combat_simulator_config config;

private:
    std::vector<double> hit_probabilities_white_mh_;
    std::vector<double> hit_probabilities_white_oh_;
    std::vector<double> hit_probabilities_yellow_;
    std::vector<double> hit_probabilities_recklessness_mh_;
    std::vector<double> hit_probabilities_recklessness_oh_;
    std::vector<double> hit_probabilities_recklessness_yellow_;
    std::vector<double> hit_probabilities_two_hand_;
    std::vector<double> hit_probabilities_recklessness_two_hand_;
    std::vector<double> batch_damage_{};
    std::vector<Damage_sources> damage_distribution_{};
    double glancing_factor_mh_{};
    double glancing_factor_oh_{};
    double armor_reduction_factor_{};
    Time_keeper time_keeper_{};
    Buff_manager buff_manager_{};
};

#include "Combat_simulator.tcc"

#endif //WOW_SIMULATOR_COMBAT_SIMULATOR_HPP