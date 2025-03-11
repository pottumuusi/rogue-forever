#ifndef DEMO_H_DEFINED
#define DEMO_H_DEFINED

struct Demo {
};

struct DemoPrivate {
    int foo;
};

struct DemoFull {
    struct Demo public;
    struct DemoPrivate private;
};

struct DemoInterface {
    int (*add_to_foo) (
        struct Demo* object_public,
        int operand);
};

void loadInterfaceDemo(void);

struct Demo* constructDemoHeap(int _foo);

void destroyDemo(struct Demo* demo_to_destroy_public);

#endif // DEMO_H_DEFINED