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
    print_stat("Htk Pwr  : ", char1.total_special_stats.attack_power, char2.total_special_stats.attack_power);
    print_stat("Haste    : ", char1.total_special_stats.haste, char2.total_special_stats.haste);
    std::cout << "\n";
    for (const auto &armor_1 : char1.armor)
    {
        for (const auto &armor_2 : char2.armor)
        {
            if (armor_1.socket == armor_2.socket)
            {
                if (armor_1.name != armor_2.name)
                {
                    std::cout << armor_1.name << " --> " << armor_2.name << "\n";
                }
            }
        }
    }

    std::cout << "\n";
}

Character character_setup(const Armory &armory, const Buffs &buffs)
{
    Character character{Race::gnome, 60};

    // Armor
    character.equip_armor(armory.helmet.crown_of_destruction);
    character.equip_armor(armory.neck.onyxia_tooth_pendant);
    character.equip_armor(armory.shoulder.leutenant_shoulders);
    character.equip_armor(armory.back.cape_of_the_black_baron);
    character.equip_armor(armory.chest.cadaverous_armor);
    character.equip_armor(armory.wrist.vambraces_of_the_sadist);
    character.equip_armor(armory.hands.edgemasters_handguards);
    character.equip_armor(armory.belt.omokks_girth);
    character.equip_armor(armory.legs.knight_captain_plate_leggings);
    character.equip_armor(armory.boots.battlechasers);
    character.equip_armor(armory.rings.don_julios_band);
    character.equip_armor(armory.rings.magnis_will);
    character.equip_armor(armory.trinket.hand_of_justice);
    character.equip_armor(armory.trinket.diamond_flask);
    character.equip_armor(armory.ranged.satyrs_bow);

    // Weapons
    character.equip_weapon(armory.swords.thrash_blade, armory.maces.ebon_hand);

    // Enchants
    character.add_enchant(Socket::head, Enchant::Type::haste);
    character.add_enchant(Socket::back, Enchant::Type::agility);
    character.add_enchant(Socket::chest, Enchant::Type::major_stats);
    character.add_enchant(Socket::wrists, Enchant::Type::strength9);
    character.add_enchant(Socket::hands, Enchant::Type::haste);
    character.add_enchant(Socket::legs, Enchant::Type::haste);
    character.add_enchant(Socket::boots, Enchant::Type::agility);
    character.add_enchant(Socket::main_hand, Enchant::Type::crusader);
    character.add_enchant(Socket::off_hand, Enchant::Type::crusader);

    character.add_buff(buffs.rallying_cry);
    character.add_buff(buffs.dire_maul);
    character.add_buff(buffs.songflower);
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

Character delta_gear(const Character &character, const Armory &armory, const Buffs &)
{
    Character delta_character = character;

//    delta_character.change_weapon(armory.axes.crul_shorukh_edge_of_chaos, Hand::main_hand);

    armory.change_armor(delta_character.armor, armory.chest.knight_captains_plate_hauberk);

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
    config.n_batches = 1000;
    config.sim_time = 60;
    config.opponent_level = 63;
    config.enable_rng_melee = false;
    config.enable_spell_rotation = true;
    config.use_heroic_spamm = false;
    config.use_mighty_rage_potion = true;
    config.enable_anger_management = true;
    config.enable_bloodrage = true;
    config.enable_talents = true;
    config.enable_item_chance_on_hit_effects = true;
    config.enable_crusader = true;
    config.enable_death_wish = true;
    config.enable_recklessness = false;
    config.display_combat_debug = false;
    config.use_seed = false;
    config.seed = 10000;
    config.fuel_extra_rage = true;
    config.extra_rage_interval = 3;
    config.extra_rage_damage_amount = 150;

    Combat_simulator simulator(config);

    std::vector<double> dps_snapshots1 = simulator.simulate(character1);
    double mean_dps1 = Combat_simulator::average(dps_snapshots1);
    double std_dps1 = Combat_simulator::standard_deviation(dps_snapshots1, mean_dps1);
    double sample_std_dps1 = Combat_simulator::sample_deviation(std_dps1, config.n_batches);

    std::cout << std::setprecision(5);
    std::cout << "Simulation results: \n";
    std::cout << mean_dps1 << " +- " << 1.96 * sample_std_dps1 << " (95% confidence interval)\n";
    std::cout << "DPS standard deviation in simulations: " << std_dps1 << "\n";
    std::cout << "Crit % left to crit cap: " << 100 - simulator.get_hit_probabilities_white_mh().back()
              << ". (Negative number means capped)\n\n";
    simulator.print_damage_distribution();

    std::vector<double> dps_snapshots2 = simulator.simulate(character2);
    double mean_dps2 = Combat_simulator::average(dps_snapshots2);
    double std_dps2 = Combat_simulator::standard_deviation(dps_snapshots2, mean_dps2);
    double sample_std_dps2 = Combat_simulator::sample_deviation(std_dps2, config.n_batches);

    std::cout << std::setprecision(5);
    std::cout << "Simulation results: \n";
    std::cout << mean_dps2 << " +- " << 1.96 * sample_std_dps2 << " (95% confidence interval)\n";
    std::cout << "DPS standard deviation in simulations: " << std_dps2 << "\n";
    std::cout << "Crit % left to crit cap: " << 100 - simulator.get_hit_probabilities_white_mh().back()
              << ". (Negative number means capped)\n\n";
    simulator.print_damage_distribution();

    std::cout << "Simulations executed in: " << double(clock() - startTime) / (double) CLOCKS_PER_SEC << " seconds."
              << std::endl;

//    for (size_t i = 0; i < characters.size(); ++i)
//    {
//        auto stat_weight_vector = simulators[i]
//                .compute_stat_weights(characters[i], armory, sim_time, opponent_level, n_batches, mean_dps[i],
//                                      sample_std_dps[i]);
//        std::cout << "Stat weights: \n";
//        for (const auto &stat_weight : stat_weight_vector)
//        {
//            std::cout << stat_weight;
//        }
//        std::cout << "\n";
//    }

    std::cout << "Code executed in: " << double(clock() - startTime) / (double) CLOCKS_PER_SEC << " seconds."
              << std::endl;

    return 0;
}

