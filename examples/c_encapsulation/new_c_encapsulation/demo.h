#ifndef DEMO_H_DEFINED
#define DEMO_H_DEFINED

struct DemoPublic {
};

struct DemoPrivate {
    int foo;
};

struct Demo {
    struct DemoPublic public;
    struct DemoPrivate private;
};

struct DemoInterface {
    int (*add_to_foo) (
        struct DemoPublic* object_public,
        int operand);
};

void loadInterfaceDemo(void);

struct DemoPublic* constructDemoHeap(int _foo);

void destroyDemo(struct DemoPublic* demo_to_destroy_public);

#endif // DEMO_H_DEFINED