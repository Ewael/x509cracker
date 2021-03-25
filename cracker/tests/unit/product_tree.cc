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
