//#include <gmpxx.h>
#include <gtest/gtest.h>

#include "batch_gcd.hh"

TEST(product_tree, one_elm)
{
    auto product_tree = cracker::compute_product_tree({ 42 });
    ASSERT_EQ(product_tree.size(), 1);
    ASSERT_EQ(product_tree[0].size(), 1);
    EXPECT_EQ(product_tree[0][0], 42);
}

TEST(product_tree, even_elm)
{
    auto product_tree = cracker::compute_product_tree({ 42, 24 });
    ASSERT_EQ(product_tree.size(), 2);
    ASSERT_EQ(product_tree[0].size(), 1);
    EXPECT_EQ(product_tree[0][0], 1008);
    ASSERT_EQ(product_tree[1].size(), 2);
    EXPECT_EQ(product_tree[1][0], 42);
    EXPECT_EQ(product_tree[1][1], 24);
}

TEST(product_tree, odd_elm)
{
    auto product_tree = cracker::compute_product_tree({ 42, 24, 3 });
    ASSERT_EQ(product_tree.size(), 3);
    ASSERT_EQ(product_tree[0].size(), 1);
    EXPECT_EQ(product_tree[0][0], 3024);
    ASSERT_EQ(product_tree[1].size(), 2);
    EXPECT_EQ(product_tree[1][0], 1008);
    EXPECT_EQ(product_tree[1][1], 3);
    ASSERT_EQ(product_tree[2].size(), 3);
    EXPECT_EQ(product_tree[2][0], 42);
    EXPECT_EQ(product_tree[2][1], 24);
    EXPECT_EQ(product_tree[2][2], 3);
}

TEST(product_tree, two_bignum)
{
    auto a = cracker::bignum(
        "86546854685346541654865436854368546845986587457986479684745879654");
    auto b = cracker::bignum(
        "354685468546853274685498654896549865748695498657468546874568"
        "5341685468574681");
    auto product_tree = cracker::compute_product_tree({ a, b });
    ASSERT_EQ(product_tree.size(), 2);
    ASSERT_EQ(product_tree[0].size(), 1);
    EXPECT_EQ(product_tree[0][0],
              cracker::bignum("306969117053285617674491905485889168328848671791"
                              "6811797736018144469048"
                              "621887782851214111155462496282586025085612091150"
                              "3980947235811493744037"
                              "4"));
}

TEST(product_tree, very_bignum)
{
    auto a = cracker::bignum("5498653468532168534163541685324986541584784574");
    auto product_tree = cracker::compute_product_tree(
        { a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a });
    ASSERT_TRUE(product_tree.size() > 1);
    ASSERT_EQ(product_tree[0].size(), 1);
    auto result = cracker::bignum(
        "6384501835027770142070954616818840540079785920636817867999057687854932"
        "9951562076349702204946174307943116084852277980428745522384805516648058"
        "7482226597963103242204286984340816237671375455766355547121284066451500"
        "2659834861855081846879065097549542608886220523042949639721547976191676"
        "1782926571464034494074650335755014223520045280948036283299959876921332"
        "4939894264432460377379759627180215962741737624415428961372889129567878"
        "7030153430966580495712200066088496573165921897930724958520032648775827"
        "3142469870582833532968479698231732929944884907616039293709973057679533"
        "6921077212844453535798261513000407311646283844772429445266396596279367"
        "4256663141514996979003400227681947704696668062133379222168072058321696"
        "0009079466658095584124012779897134658818196961779125628270505293641994"
        "1187032831121296126812871713023904127216334537996226350544589069000295"
        "8215957048040428305442947861319239532763075097598356455507619800382712"
        "05376");
    EXPECT_EQ(product_tree[0][0], result);
}