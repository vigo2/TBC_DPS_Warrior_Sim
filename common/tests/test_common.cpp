#include "find_values.hpp"

#include "gtest/gtest.h"

TEST(TestSuite, test_find_value_class)
{
    std::vector<std::string> mult_armor_vec;
    mult_armor_vec.emplace_back("destroyer_battlehelm");
    mult_armor_vec.emplace_back("pendant_of_the_perilous");
    mult_armor_vec.emplace_back("destroyer_shoulderblades");
    mult_armor_vec.emplace_back("thalassian_wildercloak");
    mult_armor_vec.emplace_back("bulwark_of_ancient_kings");
    mult_armor_vec.emplace_back("bracers_of_eradication");
    mult_armor_vec.emplace_back("destroyer_gauntlets");
    mult_armor_vec.emplace_back("belt_of_one_hundred_deaths");
    mult_armor_vec.emplace_back("destroyer_greaves");
    mult_armor_vec.emplace_back("warboots_of_obliteration");

    {
        std::vector<size_t> values{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Find_values<size_t> fv(mult_armor_vec, values);
        EXPECT_TRUE(fv.find("destroyer_battlehelm") == 0);
        EXPECT_TRUE(fv.find("pendant_of_the_perilous") == 1);
        EXPECT_TRUE(fv.find("destroyer_shoulderblades") == 2);
        EXPECT_TRUE(fv.find("thalassian_wildercloak") == 3);
        EXPECT_TRUE(fv.find("bulwark_of_ancient_kings") == 4);
        EXPECT_TRUE(fv.find("bracers_of_eradication") == 5);
        EXPECT_TRUE(fv.find("destroyer_gauntlets") == 6);
        EXPECT_TRUE(fv.find("belt_of_one_hundred_deaths") == 7);
        EXPECT_TRUE(fv.find("destroyer_greaves") == 8);
        EXPECT_TRUE(fv.find("warboots_of_obliteration") == 9);
    }
    {
        std::vector<double> values{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
        Find_values<double> fv(mult_armor_vec, values);
        EXPECT_TRUE(fv.find("destroyer_battlehelm") == 0.0);
        EXPECT_TRUE(fv.find("pendant_of_the_perilous") == 1.0);
        EXPECT_TRUE(fv.find("destroyer_shoulderblades") == 2.0);
        EXPECT_TRUE(fv.find("thalassian_wildercloak") == 3.0);
        EXPECT_TRUE(fv.find("bulwark_of_ancient_kings") == 4.0);
        EXPECT_TRUE(fv.find("bracers_of_eradication") == 5.0);
        EXPECT_TRUE(fv.find("destroyer_gauntlets") == 6.0);
        EXPECT_TRUE(fv.find("belt_of_one_hundred_deaths") == 7.0);
        EXPECT_TRUE(fv.find("destroyer_greaves") == 8.0);
        EXPECT_TRUE(fv.find("warboots_of_obliteration") == 9.0);
    }
}