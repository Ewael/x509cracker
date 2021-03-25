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

        auto product_tree = std::vector<std::vector<int>>();
        return product_tree;
    }
} // namespace cracker
