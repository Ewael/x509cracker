#include <gtest/gtest.h>

#include "batch_gcd.hh"

TEST(product_tree, one_elm)
{
    auto product_tree = cracker::compute_product_tree({ 42 });
    ASSERT_EQ(product_tree.size(), 1);
    ASSERT_EQ(product_tree[0].size(), 1);
    EXPECT_EQ(product_tree[0][0], 42);
}
