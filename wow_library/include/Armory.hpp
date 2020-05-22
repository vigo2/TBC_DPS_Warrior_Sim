#ifndef WOW_SIMULATOR_ARMORY_HPP
#define WOW_SIMULATOR_ARMORY_HPP

#include "Character.hpp"

struct Buffs
{
    // World buffs
    Buff rallying_cry{"rallying_cry", Attributes{0.0, 0.0}, Special_stats{5.0, 0.0, 140}};
    Buff dire_maul{"dire_maul", Attributes{0.0, 0.0}, Special_stats{0.0, 0.0, 200}};
    Buff songflower{"songflower", Attributes{15, 15}, Special_stats{5, 0, 0}};
    Buff warchiefs_blessing{"warchiefs_blessing", Attributes{0, 0}, Special_stats{0, 0, 0, 0, .15}};
    Buff spirit_of_zandalar{"spirit_of_zandalar", Attributes{0, 0}, Special_stats{0, 0, 0}, .15};
    Buff sayges_fortune{"sayges_fortune", Attributes{0, 0}, Special_stats{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, .1}};

    // Player_buffs
    Buff blessing_of_kings{"blessing_of_kings", Attributes{0.0, 0.0}, Special_stats{0.0, 0.0, 0.0}, .10};
    Buff blessing_of_might{"blessing_of_might", Attributes{0.0, 0.0}, Special_stats{0.0, 0.0, 155}};
    Buff gift_of_the_wild{"gift_of_the_wild", Attributes{12.0, 12.0}, Special_stats{0.0, 0.0, 0.0}};
    Buff trueshot_aura{"trueshot_aura", Attributes{0.0, 0.0}, Special_stats{0.0, 0.0, 100}};

    // Consumables
    Buff elixir_mongoose{"elixir_mongoose", Attributes{0.0, 25.0}, Special_stats{2.0, 0.0, 0.0}};
    Buff blessed_sunfruit{"blessed_sunfruit", Attributes{10.0, 0.0}, Special_stats{0.0, 0.0, 0.0}};
    Buff juju_power{"juju_power", Attributes{30.0, 0.0}, Special_stats{0.0, 0.0, 0.0}};
    Buff juju_might{"juju_might", Attributes{0.0, 0.0}, Special_stats{0.0, 0.0, 40}};
    Buff roids{"roids", Attributes{25.0, 0.0}, Special_stats{0.0, 0.0, 0.0}};

    Weapon_buff dense_stone{"dense_stone", Attributes{0.0, 0.0}, Special_stats{0.0, 0.0, 0.0}, 0, 8};
    Buff elemental_stone{"elemental_stone", Attributes{0.0, 0.0}, Special_stats{2.0, 0.0, 0.0}};
};

struct Armory
{
    std::vector<Armor> helmet_t
            {
                    {"lionheart_helm",         Attributes{18, 0},  Special_stats{2, 2, 0},             Socket::head},
                    {"expert_goldminers_head", Attributes{0, 5},   Special_stats{0, 0, 0, 0, 0, 0, 7}, Socket::head},
                    {"mask_of_the_unforgiven", Attributes{0, 0},   Special_stats{1, 2, 0},             Socket::head},
                    {"crown_of_destruction",   Attributes{0, 0},   Special_stats{2, 0, 44},            Socket::head},
                    {"helm_of_endless_rage",   Attributes{26, 26}, Special_stats{0, 0, 0},             Socket::head},
            };

    std::vector<Armor> neck_t
            {
                    {"onyxia_tooth_pendant",           Attributes{0, 13}, Special_stats{1, 1, 0},  Socket::neck},
                    {"prestors_talisman_of_connivery", Attributes{0, 30}, Special_stats{0, 1, 0},  Socket::neck},
                    {"aq_barbed_choker",               Attributes{0, 0},  Special_stats{1, 0, 44}, Socket::neck},
                    {"the_eye_of_hakkar",              Attributes{0, 0},  Special_stats{1, 0, 40}, Socket::neck},
                    {"mark_of_fordring",               Attributes{0, 0},  Special_stats{1, 0, 26}, Socket::neck},
            };

    std::vector<Armor> shoulder_t
            {
                    {"black_dragonscale_shoulders", Attributes{0, 0},   Special_stats{0, 0,
                                                                                      40},       Socket::shoulder, Set::black_dragonscale},
                    {"truestrike_shoulders",        Attributes{0, 0},   Special_stats{0, 2, 24}, Socket::shoulder},
                    {"wyrmhide_spaulders",          Attributes{0, 0},   Special_stats{0, 2, 0},  Socket::shoulder},
                    {"drake_talon_pauldrons",       Attributes{20, 20}, Special_stats{0, 0, 0},  Socket::shoulder},
                    {"leutenant_shoulders",         Attributes{17, 0},  Special_stats{1, 0,
                                                                                      0},        Socket::shoulder, Set::rare_pvp_set},
                    {"spaulders_of_valor",          Attributes{11, 9},  Special_stats{0, 0, 0},  Socket::shoulder},
                    {"aq_chitinous_shoulderguards", Attributes{11, 25}, Special_stats{0, 0, 0},  Socket::shoulder},
                    {"aq_mantle_of_wicked_revenge", Attributes{16, 30}, Special_stats{0, 0, 0},  Socket::shoulder},
                    {"aq_conquerors_spaulders",     Attributes{20, 16}, Special_stats{0, 1, 0},  Socket::shoulder},
            };

    std::vector<Armor> back_t
            {
                    {"cape_of_the_black_baron",       Attributes{0, 15},  Special_stats{0, 0, 20}, Socket::back},
                    {"cloak_of_firemaw",              Attributes{0, 0},   Special_stats{0, 0, 50}, Socket::back},
                    {"cloak_of_draconic_might",       Attributes{16, 16}, Special_stats{0, 0, 0},  Socket::back},
                    {"aq_cloak_of_concentrated_hate", Attributes{11, 16}, Special_stats{0, 1, 0},  Socket::back},
                    {"blackveil_cloak",               Attributes{6, 14},  Special_stats{0, 0, 0},  Socket::back},
            };

    std::vector<Armor> chest_t
            {
                    {"malfurions_blessed_bulwark",    Attributes{40, 0},  Special_stats{0, 0, 0},  Socket::chest},
                    {"savage_gladiator_chain",        Attributes{13, 14}, Special_stats{2, 0, 0},  Socket::chest},
                    {"knight_captains_plate_hauberk", Attributes{21, 0},  Special_stats{1, 0,
                                                                                        0},        Socket::chest, Set::rare_pvp_set},
                    {"tombstone_breastplate",         Attributes{10, 0},  Special_stats{2, 0, 0},  Socket::chest},
                    {"cadaverous_armor",              Attributes{8, 8},   Special_stats{0, 0, 60}, Socket::chest},
                    {"aq_breatplate_of_annihilation", Attributes{37, 0},  Special_stats{1, 1, 0},  Socket::chest},
                    {"aq_vest_of_swift_execution",    Attributes{21, 41}, Special_stats{0, 0, 0},  Socket::chest},
                    {"aq_conquerors_breastplate",     Attributes{34, 24}, Special_stats{0, 0, 0},  Socket::chest},
                    {"black_dragonscale_chest",       Attributes{0, 0},   Special_stats{0, 0,
                                                                                        50},       Socket::chest, Set::black_dragonscale},
                    {"breastplate_of_bloodthirst",    Attributes{10, 0},  Special_stats{2, 0, 0},  Socket::chest},
                    {"runed_bloodstaind_hauberk",     Attributes{0, 0},   Special_stats{1, 0, 58}, Socket::chest},
            };

    std::vector<Armor> wrists_t
            {
                    {"wristguards_of_stability",     Attributes{24, 0},  Special_stats{0, 0, 0}, Socket::wrist},
                    {"vambraces_of_the_sadist",      Attributes{6, 0},   Special_stats{1, 0, 0}, Socket::wrist},
                    {"wristhuards_of_true_flight",   Attributes{0, 19},  Special_stats{0, 1, 0}, Socket::wrist},
                    {"battleborn_armbraces",         Attributes{0, 0},   Special_stats{1, 1, 0}, Socket::wrist},
                    {"zandalar_armguards",           Attributes{13, 13}, Special_stats{0, 0, 0}, Socket::wrist},
                    {"blinkstrike",                  Attributes{3, 0},   Special_stats{1, 0, 0}, Socket::wrist},
                    {"aq_bracers_of_brutality",      Attributes{21, 12}, Special_stats{0, 0, 0}, Socket::wrist},
                    {"aq_hive_defiler_wristguards",  Attributes{23, 18}, Special_stats{0, 0, 0}, Socket::wrist},
                    {"aq_quiraji_execution_bracers", Attributes{15, 16}, Special_stats{0, 1, 0}, Socket::wrist},
            };

    std::vector<Armor> hands_t
            {
                    {"devilsaur_gauntlets",          Attributes{0, 0},   Special_stats{1, 0,
                                                                                       28}, Socket::hands, Set::devilsaur},
                    {"edgemasters_handguards",       Attributes{0, 0},   Special_stats{0, 0, 0, 0, 0, 7, 7,
                                                                                       7},  Socket::hands},
                    {"flameguard_gauntlets",         Attributes{0, 0},   Special_stats{1, 0,
                                                                                       54}, Socket::hands},
                    {"aq_gloves_of_enforcement",     Attributes{28, 20}, Special_stats{0, 1,
                                                                                       0},  Socket::hands},
                    {"aged_core_leather_gloves",     Attributes{15, 0},  Special_stats{1, 0, 0, 0, 0, 0, 0,
                                                                                       5},  Socket::hands},
                    {"aq_gauntlets_of_annihilation", Attributes{35, 0},  Special_stats{1, 1,
                                                                                       0},  Socket::hands},
                    {"bloodmail_gauntlets",          Attributes{9, 0},   Special_stats{1, 0,
                                                                                       0},  Socket::hands},
                    {"sacrificial_gauntlets",        Attributes{19, 0},  Special_stats{1, 1,
                                                                                       0},  Socket::hands},
            };

    std::vector<Armor> belt_t
            {
                    {"onslaught_girdle",              Attributes{31, 0},  Special_stats{1, 1, 0}, Socket::belt},
                    {"mugglers_belt",                 Attributes{0, 0},   Special_stats{1, 0, 0, 0, 0, 0, 0,
                                                                                        5},       Socket::belt},
                    {"omokks_girth",                  Attributes{15, 0},  Special_stats{1, 0,
                                                                                        0},       Socket::belt},
                    {"brigam_girdle",                 Attributes{15, 0},  Special_stats{0, 1,
                                                                                        0},       Socket::belt},
                    {"thurazane_link",                Attributes{0, 0},   Special_stats{1, 0,
                                                                                        44},      Socket::belt},
                    {"girdle_of_the_fallen_crusader", Attributes{20, 10}, Special_stats{0, 0,
                                                                                        0},       Socket::belt},
            };

    std::vector<Armor> legs_t
            {
                    {"devilsaur_leggings",                 Attributes{0, 0},   Special_stats{1, 0,
                                                                                             46},        Socket::legs, Set::devilsaur},
                    {"black_dragonscale_leggings",         Attributes{0, 0},   Special_stats{0, 0,
                                                                                             54},        Socket::legs, Set::black_dragonscale},
                    {"eldritch_legplates",                 Attributes{15, 9},  Special_stats{1, 0, 0},   Socket::legs},
                    {"ubrs_legs",                          Attributes{29, 0},  Special_stats{0, 0, 0},   Socket::legs},
                    {"cloudkeeper_legplaters",             Attributes{20, 20}, Special_stats{0, 0, 100}, Socket::legs},
                    {"legguards_of_the_fallen_crusader",   Attributes{28, 22}, Special_stats{0, 0, 0},   Socket::legs},
                    {"knight_captain_plate_leggings",      Attributes{12, 0},  Special_stats{2, 0,
                                                                                             0},         Socket::legs, Set::rare_pvp_set},
                    {"aq_legplates_of_the_qiraji_command", Attributes{20, 0},  Special_stats{2, 0, 0},   Socket::legs},
                    {"aq_scaled_sand_reaver_leggings",     Attributes{0, 0},   Special_stats{2, 0, 62},  Socket::legs},
                    {"aq_conquerors_legguards",            Attributes{33, 21}, Special_stats{0, 1, 0},   Socket::legs},
                    {"titanic_leggings",                   Attributes{30, 0},  Special_stats{1, 2, 0},   Socket::legs},
                    {"bloodsoaked_legplates",              Attributes{36, 0},  Special_stats{0, 0, 0},   Socket::legs},
            };

    std::vector<Armor> boots_t
            {
                    {"windreaver_greaves",        Attributes{0, 20},  Special_stats{0, 1, 0},  Socket::boots},
                    {"bloodmail_boots",           Attributes{9, 9},   Special_stats{0, 1, 0},  Socket::boots},
                    {"black_dragonscale_boots",   Attributes{0, 0},   Special_stats{0, 0,
                                                                                    28},       Socket::boots, Set::black_dragonscale},
                    {"knight_leutenants_greaves", Attributes{10, 9},  Special_stats{0, 0,
                                                                                    40},       Socket::boots, Set::rare_pvp_set},
                    {"battlechasers",             Attributes{14, 13}, Special_stats{0, 0, 0},  Socket::boots},
                    {"chromatic_boots",           Attributes{20, 20}, Special_stats{0, 1, 0},  Socket::boots},
                    {"boots_of_shadow_flame",     Attributes{0, 0},   Special_stats{0, 2, 44}, Socket::boots},
                    {"aq_boots_of_the_vanguard",  Attributes{22, 22}, Special_stats{0, 0, 0},  Socket::boots},
            };

    std::vector<Armor> ring_t
            {
                    {"don_julios_band",           Attributes{0, 0},   Special_stats{1, 1, 16}, Socket::ring},
                    {"magnis_will",               Attributes{6, 0},   Special_stats{1, 0, 0},  Socket::ring},
                    {"quick_strike_ring",         Attributes{8, 0},   Special_stats{1, 0, 30}, Socket::ring},
                    {"master_dragonslayers_ring", Attributes{0, 0},   Special_stats{0, 1, 48}, Socket::ring},
                    {"circle_of_applied_force",   Attributes{12, 22}, Special_stats{0, 0, 0},  Socket::ring},
                    {"blackstone_ring",           Attributes{0, 0},   Special_stats{0, 1, 20}, Socket::ring},
                    {"tarnished_elven",           Attributes{0, 15},  Special_stats{0, 1, 0},  Socket::ring},
            };

    std::vector<Armor> trinket_t
            {
                    {"hand_of_justice",     Attributes{0, 0},  Special_stats{0, 0, 20, .02}, Socket::trinket},
                    {"diamond_flask",       Attributes{75, 0}, Special_stats{0, 0, 0},       Socket::trinket},
                    {"blackhands_breadth",  Attributes{0, 0},  Special_stats{2, 0, 0},       Socket::trinket},
                    {"drake_fang_talisman", Attributes{0, 0},  Special_stats{0, 2, 56},      Socket::trinket},
            };

    std::vector<Armor> ranged_t
            {
                    {"satyrs_bow",                     Attributes{0, 3},  Special_stats{0, 1, 0},  Socket::ranged},
                    {"precisely_calibrated_boomstick", Attributes{0, 14}, Special_stats{0, 0, 0},  Socket::ranged},
                    {"strikers_mark",                  Attributes{0, 0},  Special_stats{0, 1, 22}, Socket::ranged},
                    {"heartstriker",                   Attributes{0, 0},  Special_stats{0, 0, 24}, Socket::ranged},
                    {"blastershot",                    Attributes{0, 0},  Special_stats{1, 0, 0},  Socket::ranged},
                    {"dragonbreath_hand_cannon",       Attributes{0, 14}, Special_stats{0, 0, 0},  Socket::ranged},
                    {"aq_crossbow_of_imminent_doom",   Attributes{5, 7},  Special_stats{0, 1, 0},  Socket::ranged},
                    {"aq_larvae_of_the_great_worm",    Attributes{0, 0},  Special_stats{1, 0, 18}, Socket::ranged},
                    {"bloodseeker",                    Attributes{8, 7},  Special_stats{0, 0, 0},  Socket::ranged},
                    {"riphook",                        Attributes{0, 0},  Special_stats{0, 0, 22}, Socket::ranged},
            };

    std::vector<Weapon> swords_t
            {
                    {"maladath",                     Attributes{0.0, 0.0},   Special_stats{0.0, 0.0, 0.0, 0, 0,
                                                                                           4},   2.2, 86.0,  162.0, Weapon_socket::one_hand,  Weapon_type::sword},
                    {"chromatically_tempered_sword", Attributes{14.0, 14.0}, Special_stats{0.0, 0.0,
                                                                                           0.0}, 2.6, 106.0, 198.0, Weapon_socket::one_hand,  Weapon_type::sword},
                    {"brutality_blade",              Attributes{9, 9},       Special_stats{1, 0,
                                                                                           0},   2.5, 90,    168,   Weapon_socket::one_hand,  Weapon_type::sword},
                    {"thrash_blade",                 Attributes{0, 0},       Special_stats{0, 0,
                                                                                           0},   2.7, 66,    124,   Weapon_socket::one_hand,  Weapon_type::sword, {{"thrash_blade", Hit_effect::Type::extra_hit,       {}, {}, 0,   0, 0.038}}},
                    {"assassination_blade",          Attributes{3, 0},       Special_stats{1, 0,
                                                                                           0},   2.7, 71,    132,   Weapon_socket::one_hand,  Weapon_type::sword},
                    {"mirahs_song",                  Attributes{9, 9},       Special_stats{0, 0,
                                                                                           0},   1.8, 57,    87,    Weapon_socket::one_hand,  Weapon_type::sword},
                    {"viskag",                       Attributes{0, 0},       Special_stats{0, 0,
                                                                                           0},   2.6, 100,   187,   Weapon_socket::one_hand,  Weapon_type::sword, {{"viskag",       Hit_effect::Type::damage_physical, {}, {}, 240, 0, 0.06}}},
                    {"quel_serrar",                  Attributes{0, 0},       Special_stats{0, 0,
                                                                                           0},   2.0, 84,    126,   Weapon_socket::one_hand,  Weapon_type::sword},
                    {"aq_ancient_quiraj_ripper",     Attributes{0, 0},       Special_stats{1, 0,
                                                                                           20},  2.8, 114,   213,   Weapon_socket::one_hand,  Weapon_type::sword},
                    {"dal_rends_sacred_charge",      Attributes{4, 0},       Special_stats{1, 0,
                                                                                           0},   2.8, 81,    151,   Weapon_socket::main_hand, Weapon_type::sword, std::vector<Hit_effect>(), Set::dal_rends},
                    {"dal_rends_tribal_guardian",    Attributes{0, 0},       Special_stats{0, 0,
                                                                                           0},   1.8, 52,    97,    Weapon_socket::off_hand,  Weapon_type::sword, std::vector<Hit_effect>(), Set::dal_rends},
                    {"warblade_hakkari_mh",          Attributes{0, 0},       Special_stats{1, 0,
                                                                                           28},  1.7, 59,    110,   Weapon_socket::main_hand, Weapon_type::sword, std::vector<Hit_effect>(), Set::warblade_of_the_hakkari},
                    {"warblade_hakkari_oh",          Attributes{0, 0},       Special_stats{0, 0,
                                                                                           40},  1.7, 57,    106,   Weapon_socket::off_hand,  Weapon_type::sword, std::vector<Hit_effect>(), Set::warblade_of_the_hakkari},
                    {"grand_marshals_swiftblade",    Attributes{0, 0},       Special_stats{1, 0,
                                                                                           28},  1.8, 85,    129,   Weapon_socket::one_hand,  Weapon_type::sword},
                    {"grand_marshals_longsword",     Attributes{0, 0},       Special_stats{1, 0,
                                                                                           28},  2.9, 138,   209,   Weapon_socket::one_hand,  Weapon_type::sword},
            };

//    std::vector<Weapon> two_handed_swords_t
//            {
//                    {"ashkandi", Attributes{0.0, 0.0}, Special_stats{0.0, 0.0,
//                                                                     86.0}, 3.5, 229.0, 344.0, Weapon_socket::two_hand, Weapon_type::sword},
//            };

    std::vector<Weapon> axes_t
            {
                    {"deathbringer",               Attributes{0, 0},      Special_stats{0, 0,
                                                                                        0},    2.9, 114,   213,   Weapon_socket::one_hand, Weapon_type::axe, {{"deathbringer",          Hit_effect::Type::damage_magic, {}, {}, 125, 0, 0.08}}},
                    {"crul_shorukh_edge_of_chaos", Attributes{0.0, 0.0},  Special_stats{0.0, 0.0,
                                                                                        36.0}, 2.3, 101.0, 188.0, Weapon_socket::one_hand, Weapon_type::axe},
                    {"dooms_edge",                 Attributes{9.0, 16.0}, Special_stats{0.0, 0.0,
                                                                                        0.0},  2.3, 83.0,  154.0, Weapon_socket::one_hand, Weapon_type::axe},
                    {"axe_of_the_deep_woods",      Attributes{0.0, 0.0},  Special_stats{0.0, 0.0,
                                                                                        0.0},  2.7, 78.0,  146.0, Weapon_socket::one_hand, Weapon_type::axe, {{"axe_of_the_deep_woods", Hit_effect::Type::damage_magic, {}, {}, 108, 0, 0.036}}},
                    {"ancient_hakkari_manslayer",  Attributes{0.0, 0.0},  Special_stats{0.0, 0.0,
                                                                                        0.0},  2.0, 69.0,  130.0, Weapon_socket::one_hand, Weapon_type::axe, {{"axe_of_the_deep_woods", Hit_effect::Type::damage_magic, {}, {}, 51,  0, 0.036}}},
            };

    std::vector<Weapon> daggers_t
            {
                    {"core_hound_tooth", Attributes{0, 0}, Special_stats{1, 0,
                                                                         20}, 1.6, 57, 107, Weapon_socket::one_hand, Weapon_type::dagger},
                    {"aq_deaths_sting",  Attributes{0, 0}, Special_stats{0, 0, 38, 0, 0, 0, 0,
                                                                         3},  1.8, 95, 144, Weapon_socket::one_hand, Weapon_type::dagger},
            };

    std::vector<Weapon> maces_t
            {
                    {"ebon_hand",               Attributes{0.0, 0.0}, Special_stats{0.0, 0.0, 0.0},              2.3, 83.0, 154.0,
                                                                                                                                 Weapon_socket::one_hand,  Weapon_type::mace,
                            {{"ebon_hand",           Hit_effect::Type::damage_magic, {},     {},                200, 0,  0.08}}},
                    {"spineshatter",            Attributes{9.0, 0.0}, Special_stats{0.0, 0.0,
                                                                                    0.0},                        2.5, 99.0, 184.0,
                                                                                                                                 Weapon_socket::main_hand, Weapon_type::mace},
                    {"stormstike_hammer",       Attributes{15, 0},    Special_stats{0, 0,
                                                                                    0},                          2.7, 80,   150,
                                                                                                                                 Weapon_socket::one_hand,  Weapon_type::mace},
                    {"aq_sand_polished_hammer", Attributes{0, 0},     Special_stats{1, 0,
                                                                                    20},                         2.6, 97,   181,
                                                                                                                                 Weapon_socket::one_hand,  Weapon_type::mace},
                    {"empyrean_demolisher",     Attributes{0, 0},     Special_stats{0, 0,
                                                                                    0},                          2.8, 94,   175,
                                                                                                                                 Weapon_socket::one_hand,  Weapon_type::mace,
                            {{"empyrean_demolisher", Hit_effect::Type::stat_boost,   {0, 0}, Special_stats{0, 0, 0, 0,
                                                                                                           .2}, 0,
                                                                                                                     10, 2.8 / 60}}},
                    {"aq_anubisath_warhammer",  Attributes{0, 0},     Special_stats{0, 0, 32, 0, 0, 0, 0, 0,
                                                                                    4},                          1.8, 66,
                                                                                                                            123, Weapon_socket::one_hand,  Weapon_type::mace},
            };

    std::vector<Weapon> fists_t
            {
                    {"claw_of_the_black_drake", Attributes{13, 0}, Special_stats{1, 0, 0}, 2.6, 102, 191,
                            Weapon_socket::main_hand, Weapon_type::unarmed},
            };

    std::vector<Set_bonus> set_bonuses{
            {Attributes{0, 0}, Special_stats{0, 2, 0},          2, Set::devilsaur},
            {Attributes{0, 0}, Special_stats{0, 1, 0},          2, Set::black_dragonscale},
            {Attributes{0, 0}, Special_stats{2, 0, 0},          3, Set::black_dragonscale},
            {Attributes{0, 0}, Special_stats{0, 0, 40},         2, Set::rare_pvp_set},
            {Attributes{0, 0}, Special_stats{0, 0, 50},         2, Set::dal_rends},
            {Attributes{0, 0}, Special_stats{0, 0, 0, 0, 0, 6}, 2, Set::warblade_of_the_hakkari},
    };

    Armor find_armor(Socket socket, const std::string &name) const;

    Weapon find_weapon(const std::string &name) const;

    std::vector<Set_bonus> get_set_bonuses() const;

    Attributes get_enchant_attributes(Socket socket, Enchant::Type type) const;

    Special_stats get_enchant_special_stats(Socket socket, Enchant::Type type) const;

    Hit_effect enchant_hit_effect(double weapon_speed, Enchant::Type type) const;

    void clean_weapon(Weapon &weapon) const;

    void compute_total_stats(Character &character) const;

    bool check_if_armor_valid(const std::vector<Armor> &armor) const;

    bool check_if_weapons_valid(std::vector<Weapon> &weapons) const;

    void change_weapon(std::vector<Weapon> &current_weapons, const Weapon &equip_weapon, const Socket &socket) const;

    void change_armor(std::vector<Armor> &armor_vec, const Armor &armor, bool first_misc_slot = true) const;
};

#endif //WOW_SIMULATOR_ARMORY_HPP