#include "batch_gcd.hh"

namespace cracker
{
    std::vector<std::vector<bignum>>
    compute_product_tree(std::vector<bignum> input)
    {
        if (input.size() == 1)
        {
            return std::vector<std::vector<bignum>>{ std::vector<bignum>{
                input[0] } };
        }

        auto level = std::vector<bignum>();
        for (size_t i = 0; i < input.size(); i += 2)
        {
            // In case input size is odd
            if (i == input.size() - 1)
            {
                level.push_back(input[i]);
            }
            else
            {
                level.push_back(input[i] * input[i + 1]);
            }
        }

        auto product_tree = compute_product_tree(level);
        product_tree.push_back(input);
        return product_tree;
    }
} // namespace cracker
