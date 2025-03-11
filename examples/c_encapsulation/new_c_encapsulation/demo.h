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
    int (*add_to_foo) (int operand);
    int (*add_to_fooV2) (
        struct DemoPublic* object_public,
        int operand);
};

void loadInterfaceDemo(void);

struct DemoPublic* constructDemoV2Heap(int _foo);

void destroyDemoV2(struct DemoPublic* demo_to_destroy_public);

#endif // DEMO_H_DEFINED