#include "item_optimizer_fixture.cpp"
#include "string_helpers.hpp"

TEST_F(Optimizer_fixture, test_multi_select)
{
    std::vector<std::string> mult_armor_vec{};
    mult_armor_vec.emplace_back("destroyer_battlehelm");
    mult_armor_vec.emplace_back("warbringer_greathelm");

    mult_armor_vec.emplace_back("pendant_of_the_perilous");
    mult_armor_vec.emplace_back("braided_eternium_chain");

    mult_armor_vec.emplace_back("warbringer_shoulderplates");
    mult_armor_vec.emplace_back("destroyer_shoulderblades");

    mult_armor_vec.emplace_back("thalassian_wildercloak");
    mult_armor_vec.emplace_back("vengeance_wrap");

    mult_armor_vec.emplace_back("bulwark_of_kings");
    mult_armor_vec.emplace_back("bulwark_of_ancient_kings");

    mult_armor_vec.emplace_back("bladespire_warbands");
    mult_armor_vec.emplace_back("bracers_of_eradication");

    mult_armor_vec.emplace_back("gauntlets_of_martial_perfection");
    mult_armor_vec.emplace_back("destroyer_gauntlets");

    mult_armor_vec.emplace_back("belt_of_one_hundred_deaths");
    mult_armor_vec.emplace_back("girdle_of_the_endless_pit");

    std::vector<std::string> mult_weapon_vec{};
    mult_weapon_vec.emplace_back("gladiators_slicer");
    mult_weapon_vec.emplace_back("merciless_gladiators_slicer");

    std::vector<extra_option> extra_options{};
    extra_options.emplace_back("item_filter_threshold", 2500);
    extra_options.emplace_back("item_filter_timeout", 1);
    extra_options.emplace_back("min_iterations", 15);

    set_multiselect_fixture(mult_armor_vec, mult_weapon_vec, extra_options);
    set_target_items({"destroyer_battlehelm", "pendant_of_the_perilous", "destroyer_shoulderblades",
                      "thalassian_wildercloak","bulwark_of_ancient_kings", "bracers_of_eradication", 
                      "destroyer_gauntlets"});

    double success_rate = run_multiselect(10, true);

    // Allow two failed runs. This is to make it very unlikely to fail if it is working.
    EXPECT_GE(success_rate, 0.8);
}

TEST_F(Optimizer_fixture, test_multi_select_item_filter)
{
    std::vector<std::string> mult_armor_vec{};
    mult_armor_vec.emplace_back("destroyer_battlehelm");
    mult_armor_vec.emplace_back("warbringer_greathelm");

    mult_armor_vec.emplace_back("pendant_of_the_perilous");
    mult_armor_vec.emplace_back("braided_eternium_chain");

    mult_armor_vec.emplace_back("warbringer_shoulderplates");
    mult_armor_vec.emplace_back("destroyer_shoulderblades");

    mult_armor_vec.emplace_back("thalassian_wildercloak");
    mult_armor_vec.emplace_back("vengeance_wrap");

    mult_armor_vec.emplace_back("bulwark_of_kings");
    mult_armor_vec.emplace_back("bulwark_of_ancient_kings");

    mult_armor_vec.emplace_back("bladespire_warbands");
    mult_armor_vec.emplace_back("bracers_of_eradication");

    mult_armor_vec.emplace_back("gauntlets_of_martial_perfection");
    mult_armor_vec.emplace_back("destroyer_gauntlets");

    mult_armor_vec.emplace_back("belt_of_one_hundred_deaths");
    mult_armor_vec.emplace_back("girdle_of_the_endless_pit");

    mult_armor_vec.emplace_back("destroyer_greaves");
    mult_armor_vec.emplace_back("warbringer_greaves");

    mult_armor_vec.emplace_back("warboots_of_obliteration");
    mult_armor_vec.emplace_back("ironstriders_of_urgency");

    mult_armor_vec.emplace_back("band_of_the_ranger_general");
    mult_armor_vec.emplace_back("mithril_band_of_the_unscarred");
    mult_armor_vec.emplace_back("shapeshifters_signet");
    mult_armor_vec.emplace_back("veterans_band_of_triumph");

    mult_armor_vec.emplace_back("mark_of_the_champion");
    mult_armor_vec.emplace_back("dragonspine_trophy");
    mult_armor_vec.emplace_back("tsunami_talisman");

    mult_armor_vec.emplace_back("serpent_spine_longbow");
    mult_armor_vec.emplace_back("mamas_insurance");

    std::vector<std::string> mult_weapon_vec{};
    mult_weapon_vec.emplace_back("gladiators_slicer");
    mult_weapon_vec.emplace_back("merciless_gladiators_slicer");
    mult_weapon_vec.emplace_back("talon_of_azshara");
    mult_weapon_vec.emplace_back("gladiators_quickblade");

    std::vector<extra_option> extra_options{};
    extra_options.emplace_back("item_filter_threshold", 200);
    extra_options.emplace_back("item_filter_timeout", 0.5);
    extra_options.emplace_back("min_iterations", 15);

    set_multiselect_fixture(mult_armor_vec, mult_weapon_vec, extra_options);
    set_target_items({"destroyer_battlehelm", "pendant_of_the_perilous", "destroyer_shoulderblades",
                      "bulwark_of_ancient_kings", "bracers_of_eradication", "destroyer_gauntlets",
                      "belt_of_one_hundred_deaths", "destroyer_greaves", "warboots_of_obliteration",
                      "band_of_the_ranger_general", "veterans_band_of_triumph", "tsunami_talisman", "mark_of_the_champion",
                      "mamas_insurance", "merciless_gladiators_slicer", "talon_of_azshara"});

    double success_rate = run_multiselect(10, true);

    // Allow two failed runs. This is to make it very unlikely to fail if it is working.
    EXPECT_GE(success_rate, 0.8);
}
