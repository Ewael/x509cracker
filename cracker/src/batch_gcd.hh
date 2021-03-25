#pragma once

#include <gmpxx.h>
#include <vector>

namespace cracker
{
    using bignum = mpz_class;

    std::vector<std::vector<bignum>>
    compute_product_tree(std::vector<bignum> input);
} // namespace cracker
