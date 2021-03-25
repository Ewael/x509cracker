#include <vector>

namespace cracker
{
    std::vector<std::vector<int>> compute_product_tree(std::vector<int> input)
    {
        if (input.size() == 1)
        {
            return std::vector<std::vector<int>>{ std::vector<int>{
                input[0] } };
        }

        auto level = std::vector<int>();
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
        product_tree.push_back(level);
        return product_tree;
    }
} // namespace cracker
