#ifndef DEMO_H_DEFINED
#define DEMO_H_DEFINED

struct DemoPublic {
    struct DemoPublic* this_instance; // TODO remove if unnecessary

    void (*load_function_context) (struct DemoPublic* this_instance);
    void (*unload_function_context) (void);
    int (*add_to_foo) (int operand);
};

struct DemoPrivate {
    struct DemoPublic* owner;

    int foo;
};

// Instances not to be exposed outside module.c as it would expose private
// struct.
struct Demo {
    struct DemoPublic* public;
    struct DemoPrivate* private;
};

struct DemoPublic* constructDemo(int _foo);

void destroyDemo(struct DemoPublic* module_to_destroy);

#endif // DEMO_H_DEFINED
