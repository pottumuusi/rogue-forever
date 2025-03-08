#include "Module.hpp"

Module::~Module()
{
}

Module::Module(int _foo)
    : foo{_foo}
{
    if (1 == _foo) {
        foo = 1 + 1;
        return;
    }

    foo = _foo;
}

int Module::addToFoo(int operand)
{
    return foo + operand;
}

int Module::getBeacon(void)
{
    return beacon;
}