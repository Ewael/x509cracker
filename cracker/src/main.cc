#include <gmpxx.h>
#include <iostream>

int main()
{
    mpz_class a, b;
    a = "86546854685346541654865436854368546845986587457986479684745879654";
    b = "3546854685468532746854986548965498657486954986574685468745685341685468"
        "574681";
    std::cout << a * b << std::endl;
    return 0;
}
