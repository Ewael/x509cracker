#include <gmpxx.h>
#include <iostream>

#include "batch_gcd.hh"
#include "database.hh"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <database path>" << std::endl;
        return 1;
    }

    auto modulos = cracker::get_modulos_from_file(argv[1]);
    std::cout << "Got " << modulos.size() << " modulos to compute" << std::endl;

    auto gcds = cracker::compute_all(modulos);

    return 0;
}
