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
}

TEST(product_tree, odd_elm)
{
    auto product_tree = cracker::compute_product_tree({ 42, 24, 3 });
    ASSERT_EQ(product_tree.size(), 3);
    ASSERT_EQ(product_tree[0].size(), 1);
    EXPECT_EQ(product_tree[0][0], 3024);
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
