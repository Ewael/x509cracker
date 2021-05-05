#pragma once

#include <gmpxx.h>
#include <vector>

namespace cracker
{
    using bignum = mpz_class;

    std::vector<std::vector<bignum>>
    compute_product_tree(std::vector<bignum> &modulos);

    std::vector<bignum>
    compute_remainders(std::vector<std::vector<bignum>> &product_tree);

    std::vector<bignum> compute_gcds(std::vector<bignum> modulos,
                                     std::vector<bignum> &remainders);

    std::vector<bignum> compute_all(std::vector<bignum> modulos);
} // namespace cracker
