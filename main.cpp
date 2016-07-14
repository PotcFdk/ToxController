#define BOOTSTRAP_ADDRESS "23.226.230.47"
#define BOOTSTRAP_PORT 33445
#define BOOTSTRAP_KEY "A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074"

#include "example/example.hpp"

int main()
{
    ExampleClass ec (BOOTSTRAP_ADDRESS, BOOTSTRAP_PORT, BOOTSTRAP_KEY, "Example", "C++ Tox client using ToxController");

    std::cout << ec.getAddress() << std::endl;

    for (;;)
        ec.think();

    return 0;
}
