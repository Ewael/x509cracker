#include "batch_gcd.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace cracker
{
    // Check suffix of string
    inline bool ends_with(std::string const &value, std::string const &ending)
    {
        if (ending.size() > value.size())
            return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    std::vector<bignum> get_modulos_from_dir(char *path, std::vector<std::string> &filenames)
    {
        std::vector<bignum> modulos;

        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            // Filtering only .hex files
            if (!ends_with(entry.path(), ".hex"))
                continue;
            // Reading whole file
            std::ifstream file(entry.path());
            filenames.push_back(entry.path());
            std::stringstream hex_key;
            hex_key << file.rdbuf();
            // Add key
            bignum n;
            mpz_set_str(n.get_mpz_t(), hex_key.str().c_str(), 16);
            modulos.push_back(n);
        }

        return modulos;
    }
}