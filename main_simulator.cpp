#include <iostream>
#include <ctime>

#include "wow_library/include/Combat_simulator.hpp"
#include "wow_library/include/Armory.hpp"

// TODO move weapon mechanics to the simulator instead of weapon class
// TODO Graphics?
// TODO HOJ proc when whirlwind etc.
// TODO Print the same data that is shown in warcraftlogs
// TODO 2h weapons

// TODO anger management
// TODO blood rage
// TODO deep wounds

// TODO move battle shout in the simulation
// TODO cooldowns
// TODO stances
// TODO overpower

// TODO setbonuses

// Notes
// Execute and bloodthirst refunds rage on dodge, not whirlwind
// Offhand hit chance is increased if heroic strike is activated

void print_stat(const std::string &stat_name, double amount_1, double amount_2)
{
    std::cout << stat_name << std::setw(6) << std::left << std::setprecision(6)
              << amount_1 << " --> " << std::setw(6) << amount_2 << "\n";
}

void print_character_stat(const Character &char1, const Character &char2)
{
    std::cout << "Character1        -->" << " Character2" << "\n";
    print_stat("Strength : ", char1.total_attributes.strength, char2.total_attributes.strength);
    print_stat("Agility  : ", char1.total_attributes.agility, char2.total_attributes.agility);
    print_stat("Hit:     : ", char1.total_special_stats.hit, char2.total_special_stats.hit);
    print_stat("Crit:    : ", char1.total_special_stats.critical_strike, char2.total_special_stats.critical_strike);
    print_stat("Atk Pwr  : ", char1.total_special_stats.attack_power, char2.total_special_stats.attack_power);
    print_stat("Haste    : ", char1.total_special_stats.haste, char2.total_special_stats.haste);
    print_stat("Swrd skil: ", char1.total_special_stats.sword_skill, char2.total_special_stats.sword_skill);
    print_stat("Axe  skil: ", char1.total_special_stats.axe_skill, char2.total_special_stats.axe_skill);
    print_stat("Dagr skil: ", char1.total_special_stats.dagger_skill, char2.total_special_stats.dagger_skill);
    print_stat("Mace skil: ", char1.total_special_stats.mace_skill, char2.total_special_stats.mace_skill);
    print_stat("Unrm skil: ", char1.total_special_stats.fist_skill, char2.total_special_stats.fist_skill);
    std::cout << "\n";

    std::cout << "Armor:" << "\n";
    for (size_t i = 0; i < char1.armor.size(); i++)
    {
        if (char1.armor[i].name != char2.armor[i].name)
        {
            std::cout << char1.armor[i].name << " --> " << char2.armor[i].name << "\n";
        }
    }

    std::cout << "\n" << "Weapons:" << "\n";
    for (size_t i = 0; i < char1.weapons.size(); i++)
    {
        if (char1.weapons[i].name != char2.weapons[i].name)
        {
            if (i == 0)
            {
                std::cout << "Mainhand: ";
            }
            else
            {
                std::cout << "Offhand: ";
            }
            std::cout << char1.weapons[i].name << " --> " << char2.weapons[i].name << "\n";
        }
    }

    std::cout << "\n";
}

Character character_setup(const Armory &armory, const Buffs &buffs)
{
    Character character{Race::human, 60};

    character.equip_armor(armory.find_armor(Socket::head, "lionheart_helm"));
    character.equip_armor(armory.find_armor(Socket::neck, "onyxia_tooth_pendant"));
    character.equip_armor(armory.find_armor(Socket::shoulder, "drake_talon_pauldrons"));
    character.equip_armor(armory.find_armor(Socket::back, "cape_of_the_black_baron"));
    character.equip_armor(armory.find_armor(Socket::chest, "savage_gladiator_chain"));
    character.equip_armor(armory.find_armor(Socket::wrist, "wristguards_of_stability"));
    character.equip_armor(armory.find_armor(Socket::hands, "flameguard_gauntlets"));
    character.equip_armor(armory.find_armor(Socket::belt, "onslaught_girdle"));
    character.equip_armor(armory.find_armor(Socket::legs, "legguards_of_the_fallen_crusader"));
    character.equip_armor(armory.find_armor(Socket::boots, "chromatic_boots"));
    character.equip_armor(armory.find_armor(Socket::ring, "don_julios_band"));
    character.equip_armor(armory.find_armor(Socket::ring, "master_dragonslayers_ring"));
    character.equip_armor(armory.find_armor(Socket::trinket, "hand_of_justice"));
    character.equip_armor(armory.find_armor(Socket::trinket, "diamond_flask"));
    character.equip_armor(armory.find_armor(Socket::ranged, "blastershot"));

    character.equip_weapon(armory.find_weapon("empyrean_demolisher"), armory.find_weapon("brutality_blade"));

    // Enchants
    character.add_enchant(Socket::head, Enchant::Type::haste);
    character.add_enchant(Socket::back, Enchant::Type::agility);
    character.add_enchant(Socket::chest, Enchant::Type::minor_stats);
    character.add_enchant(Socket::wrist, Enchant::Type::strength7);
    character.add_enchant(Socket::hands, Enchant::Type::haste);
    character.add_enchant(Socket::legs, Enchant::Type::haste);
    character.add_enchant(Socket::boots, Enchant::Type::agility);
    character.add_enchant(Socket::main_hand, Enchant::Type::crusader);
    character.add_enchant(Socket::off_hand, Enchant::Type::crusader);

    character.add_buff(buffs.rallying_cry);
    character.add_buff(buffs.dire_maul);
//    character.add_buff(buffs.songflower);
    character.add_buff(buffs.spirit_of_zandalar);
    character.add_buff(buffs.sayges_fortune);
//    character.add_buff(buffs.warchiefs_blessing);
    character.add_buff(buffs.blessing_of_kings);
    character.add_buff(buffs.blessing_of_might);
    character.add_buff(buffs.gift_of_the_wild);
    character.add_buff(buffs.trueshot_aura);
    character.add_buff(buffs.elixir_mongoose);
    character.add_buff(buffs.blessed_sunfruit);
    character.add_buff(buffs.juju_power);
    character.add_buff(buffs.juju_might);
    character.add_buff(buffs.roids);

    character.add_weapon_buff(Socket::main_hand, buffs.dense_stone);
    character.add_weapon_buff(Socket::off_hand, buffs.dense_stone);

    armory.compute_total_stats(character);

    return character;
}

Character delta_gear(const Character &character, const Armory &armory, const Buffs &buffs)
{
    auto __attribute__((unused)) placeholder = buffs.songflower;
    Character delta_character = character;
//    delta_character.add_buff(buffs.sayges_fortune); // 11.8
//    delta_character.add_buff(buffs.warchiefs_blessing); // 11.4
//    delta_character.add_buff(buffs.rallying_cry); // 12.5
//    delta_character.add_buff(buffs.dire_maul); // 7.5
//    delta_character.add_buff(buffs.spirit_of_zandalar); // 7.8
//    delta_character.add_buff(buffs.songflower); // 9.3

    armory.change_weapon(delta_character.weapons, armory.find_weapon("maladath"), Socket::main_hand);

//    armory.change_armor(delta_character.armor, armory.ranged.strikers_mark);

//    delta_character.add_enchant(Socket::head, Enchant::Type::haste);

    armory.compute_total_stats(delta_character);

    return delta_character;
}

int main()
{
    clock_t startTime = clock();
    Armory armory{};
    Buffs buffs{};

    Character character1 = character_setup(armory, buffs);
    Character character2 = delta_gear(character1, armory, buffs);

    print_character_stat(character1, character2);

    // Simulator & Combat settings
    Combat_simulator_config config{};
    config.n_batches = 10000;
    config.sim_time = 60;
    config.opponent_level = 63;

    config.curse_of_recklessness_active = true;
    config.faerie_fire_feral_active = true;
    config.sunder_armor_active = true;
    config.n_sunder_armor_stacks = 5;

    config.talents.improved_heroic_strike = 2;
    config.talents.unbridled_wrath = 5;
    config.talents.flurry = 5;
    config.talents.anger_management = true;
    config.talents.death_wish = true;
    config.talents.impale = 2;
    config.talents.improved_execute = 2;
    config.talents.dual_wield_specialization = 5;

    config.use_sim_time_ramp = true;
    config.enable_spell_rotation = true;
    config.use_mighty_rage_potion = true;
    config.enable_bloodrage = true;
    config.enable_recklessness = true;
//    config.display_combat_debug = true;
    config.use_seed = true;
    config.seed = 110000;
    config.fuel_extra_rage = false;
    config.extra_rage_interval = 3;
    config.extra_rage_damage_amount = 150;

    Combat_simulator simulator(config);

    std::cout << "Simulation settings: \n";
    std::cout << "Opponent level: " << config.opponent_level << "\n";
    std::cout << "Simulation time: " << config.sim_time << " seconds.\n\n";
//    std::cout << "Opponent level: " << config.opponent_level << "\n";

    std::vector<double> dps_snapshots1 = simulator.simulate(character1);
    double mean_dps1 = Statistics::average(dps_snapshots1);
    double std_dps1 = Statistics::standard_deviation(dps_snapshots1, mean_dps1);
    double sample_std_dps1 = Statistics::sample_deviation(std_dps1, config.n_batches);

    std::cout << std::setprecision(5);
    std::cout << "Simulation results Character 1: \n";
    std::cout << mean_dps1 << " +- " << 1.96 * sample_std_dps1 << " (95% confidence interval)\n";
    std::cout << "DPS standard deviation in simulations: " << std_dps1 << "\n";
    std::cout << "Crit % left to crit cap: " << 100 - simulator.get_hit_probabilities_white_mh().back()
              << ". (Negative number means capped)\n\n";
    simulator.print_damage_distribution();

    auto aura_uptimes = simulator.get_aura_uptimes();
    std::cout << "\nAura uptimes Character 1: \n";
    for (const auto &aura : aura_uptimes)
    {
        std::cout << aura << "\n";
    }
    std::cout << "\n\n";

    std::vector<double> dps_snapshots2 = simulator.simulate(character2);
    double mean_dps2 = Statistics::average(dps_snapshots2);
    double std_dps2 = Statistics::standard_deviation(dps_snapshots2, mean_dps2);
    double sample_std_dps2 = Statistics::sample_deviation(std_dps2, config.n_batches);

    std::cout << std::setprecision(5);
    std::cout << "Simulation results Character 2: (" << 100.0 * mean_dps2 / mean_dps1 << "%)" << "\n";
    std::cout << mean_dps2 << " +- " << 1.96 * sample_std_dps2 << " (95% confidence interval)\n";
    std::cout << "DPS standard deviation in simulations: " << std_dps2 << "\n";
    std::cout << "Crit % left to crit cap: " << 100 - simulator.get_hit_probabilities_white_mh().back()
              << ". (Negative number means capped)\n\n";
    simulator.print_damage_distribution();

    aura_uptimes = simulator.get_aura_uptimes();
    std::cout << "\nAura uptimes Character 1: \n";
    for (const auto &aura : aura_uptimes)
    {
        std::cout << aura << "\n";
    }
    std::cout << "\n";

    std::cout << "Simulations executed in: " << double(clock() - startTime) / (double) CLOCKS_PER_SEC << " seconds."
              << std::endl;

    std::cout << "Code executed in: " << double(clock() - startTime) / (double) CLOCKS_PER_SEC << " seconds."
              << std::endl;

    return 0;
}
