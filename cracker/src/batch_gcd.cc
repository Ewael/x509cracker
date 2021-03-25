#include "batch_gcd.hh"

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
    compute_product_tree(std::vector<bignum> &modulos)
    {
        if (modulos.size() == 1)
        {
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
                level.push_back(modulos[i] * modulos[i + 1]);
            }
        }

        auto product_tree = compute_product_tree(level);
        product_tree.push_back(modulos);
        return product_tree;
    }

    std::vector<bignum>
    compute_remainders(std::vector<std::vector<bignum>> &product_tree)
    {
        std::vector<bignum> remainders = product_tree.front();
        product_tree.erase(product_tree.begin());
        while (product_tree.size() > 0)
        {
            std::vector<bignum> level = product_tree.front();
            product_tree.erase(product_tree.begin());
            std::vector<bignum> next_remainders = std::vector<bignum>();
            for (size_t i = 0; i < level.size(); i++)
            {
                next_remainders.push_back(remainders[floor(i / 2)]
                                          % (level[i] * level[i]));
            }
            remainders = next_remainders;
        }

        return remainders;
    }

    std::vector<bignum> compute_gcds(std::vector<bignum> modulos,
                                     std::vector<bignum> &remainders)
    {
        auto gcds = std::vector<bignum>();
        for (size_t i = 0; i < max(modulos.size(), remainders.size()); i++)
        {
            bignum tmp;
            bignum div = remainders[i] / modulos[i];
            mpz_gcd(tmp.get_mpz_t(), div.get_mpz_t(), modulos[i].get_mpz_t());
            gcds.push_back(tmp);
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
