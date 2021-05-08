#include <gmpxx.h>
#include <iostream>
#include <string.h>

#include "batch_gcd.hh"
#include "database.hh"
#include "from_dir.hh"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " [ --db <database path> | --from-dir <directory path]" << std::endl;
        return 1;
    }

    std::vector<cracker::bignum> modulos;
    if (strcmp("--db", argv[1]) == 0)
    {
         modulos = cracker::get_modulos_from_file(argv[2]);
         std::cout << "Got " << modulos.size() << " modulos to compute" << std::endl;
         auto gcds = cracker::compute_all(modulos);
    }
    else if (strcmp("--from-dir", argv[1]) == 0)
    {
        std::vector<std::string> filenames;
        modulos = cracker::get_modulos_from_dir(argv[2], filenames);
        std::cout << "Got " << modulos.size() << " modulos to compute" << std::endl;
        auto gcds = cracker::compute_all(modulos);
        std::cout << "Corresponding files:" << std::endl;
        for (size_t i = 0; i < gcds.size(); i++)
        {
            if (gcds[i] != 1)
                std::cout << filenames[i] << std::endl;
        }
    }
    else
    {
        std::cerr << "Wrong parameter" << std::endl;
        exit(1);
    }

    return 0;
}
