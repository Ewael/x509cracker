#include <gmpxx.h>
#include <iostream>

#include "batch_gcd.hh"

int main()
{
    std::vector<cracker::bignum> modulos{ 1909, 2923, 291,  205,  989,
                                          62,   451,  1943, 1079, 2419 };
    auto res = cracker::compute_all(modulos);
    for (auto &i : res)
    {
        std::cout << i << std::endl;
    }
    return 0;
}
