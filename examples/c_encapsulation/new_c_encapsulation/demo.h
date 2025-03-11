#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

struct ModulePublic {
};

struct ModulePrivate {
    int foo;
};

struct Module {
    struct ModulePublic public;
    struct ModulePrivate private;
};

struct ModuleInterface {
    int (*add_to_foo) (int operand);
    int (*add_to_fooV2) (
        struct ModulePublic* object_public,
        int operand);
};

void loadInterfaceModule(void);

struct ModulePublic* constructModuleV2Heap(int _foo);

void destroyModuleV2(struct ModulePublic* module_to_destroy_public);

#endif // MODULE_H_DEFINED