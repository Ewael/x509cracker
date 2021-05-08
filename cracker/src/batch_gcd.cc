#include "batch_gcd.hh"

#include <iostream>
#include <cassert>
#include <math.h>

namespace cracker
{
    size_t max(size_t a, size_t b)
    {
        if (a >= b)
            return a;
        return b;
    }

    std::vector<std::vector<bignum>>
    compute_product_tree_rec(std::vector<bignum> &modulos)
    {
        if (modulos.size() == 1)
        {
            // Ending recursion, we have only one element: the result of the product
            return std::vector<std::vector<bignum>>{ std::vector<bignum>{
                modulos[0] } };
        }

        auto level = std::vector<bignum>();
        for (size_t i = 0; i < modulos.size(); i += 2)
        {
            // In case input size is odd
            if (i == modulos.size() - 1)
            {
                level.push_back(modulos[i]);
            }
            else
            {
                // Adding the product of the two leaves
                level.push_back(modulos[i] * modulos[i + 1]);
            }
        }

        auto product_tree = compute_product_tree_rec(level);
        product_tree.push_back(modulos);
        return product_tree;
    }

    std::vector<std::vector<bignum>>
    compute_product_tree(std::vector<bignum> &modulos)
    {
        assert(modulos.size() > 0);
        std::cout << "Computing product tree" << std::endl;
        return compute_product_tree_rec(modulos);
    }

    std::vector<bignum>
    compute_remainders(std::vector<std::vector<bignum>> &product_tree)
    {
        std::cout << "Computing remainders" << std::endl;

        std::vector<bignum> remainders = product_tree[0];
        for (size_t k = 1; k < product_tree.size(); k++)
        {
            std::vector<bignum> next_remainders = std::vector<bignum>();
            for (size_t i = 0; i < product_tree[k].size(); i++)
            {
                next_remainders.push_back(
                    remainders[floor(i / 2)]
                    % (product_tree[k][i] * product_tree[k][i]));
            }
            remainders = next_remainders;
        }

        return remainders;
    }

    std::vector<bignum> compute_gcds(std::vector<bignum> modulos,
                                     std::vector<bignum> &remainders)
    {
        std::cout << "Computing gcds" << std::endl;

        auto gcds = std::vector<bignum>();
        for (size_t i = 0; i < max(modulos.size(), remainders.size()); i++)
        {
            bignum result;
            bignum div = remainders[i] / modulos[i];
            // Compute gcd(r/n, n)
            mpz_gcd(result.get_mpz_t(), div.get_mpz_t(),
                    modulos[i].get_mpz_t());

            if (result != 1)
            {
                std::cout << "Factorization possible at index " << i << std::endl;
            }

            gcds.push_back(result);
        }
        return gcds;
    }

    std::vector<bignum> compute_all(std::vector<bignum> modulos)
    {
        auto prod_tree = compute_product_tree(modulos);
        auto remainders = compute_remainders(prod_tree);
        return compute_gcds(modulos, remainders);
    }

} // namespace cracker
