#include "Module.hpp"

#include <stdio.h>

#if SLEEP_AT_END
#include <unistd.h>
#endif

extern "C" int main(int argc, char* argv[])
{
#if MULTIPLE_MODULES
    Module module_array[2] = {0};
    module_array[0] = Module(222);
    module_array[1] = Module(888);
#else
    Module module = Module(222);
#endif

    int result = 0;
    int bar = 4444;

#if MULTIPLE_MODULES
    result = module_array[0].addToFoo(bar);
#else
    result = module.addToFoo(bar);
#endif

#if 0
    printf("Trying to directly access private variable: %d\n", module.beacon);
#endif

    printf("result is: %d\n", result);
#if MULTIPLE_MODULES
    printf("getBeacon() is: %d\n", module_array[1].getBeacon());
#else
    printf("getBeacon() is: %d\n", module.getBeacon());
#endif

#if SLEEP_AT_END
    while (1) {
        sleep(1);
    }
#endif

    return 0;
}
