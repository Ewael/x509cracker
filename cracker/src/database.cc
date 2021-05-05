#include <sqlite3.h>

#include "batch_gcd.hh"

namespace cracker
{
    int request_callback(void *data, int argc, char **argv, char **azColName)
    {
        std::vector<bignum> *modulos = static_cast<std::vector<bignum> *>(data);
        (void)azColName;
        for (int i = 0; i < argc; i++)
        {
            if (!argv[i])
                continue;
            bignum n;
            mpz_set_str(n.get_mpz_t(), argv[i] + 2, 16);
            modulos->push_back(n);
        }
        return 0;
    }

    std::vector<bignum> get_modulos_from_file(char *filename)
    {
        sqlite3 *db;
        std::vector<bignum> modulos;

        if (sqlite3_open(filename, &db))
            throw std::invalid_argument("Could not open database.");
        std::string request("SELECT modulus FROM certificates");

        int ret = sqlite3_exec(db, request.c_str(), request_callback,
                               static_cast<void *>(&modulos), NULL);
        sqlite3_close(db);
        if (ret != SQLITE_OK)
            throw std::string("Request failed.");
        return modulos;
    }
} // namespace cracker