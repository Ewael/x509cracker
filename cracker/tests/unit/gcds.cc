#include <gtest/gtest.h>

#include "batch_gcd.hh"

TEST(gcds, basic)
{
    std::vector<cracker::bignum> modulos{ 1909, 2923, 291,  205,  989,
                                          62,   451,  1943, 1079, 2419 };
    auto gcds = cracker::compute_all(modulos);

    std::vector<cracker::bignum> expected{
        1909, 1, 1, 41, 23, 1, 41, 1, 83, 41
    };

    EXPECT_EQ(gcds, expected);
}