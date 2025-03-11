#ifndef MODULE_HPP_DEFINED
#define MODULE_HPP_DEFINED

class Module {
public:
    Module() = default;
    ~Module();
    Module(int _foo);

    int addToFoo(int operand);
    int getBeacon(void);

private:
    int foo;
    int beacon = 5555;
};

#endif // MODULE_HPP_DEFINED