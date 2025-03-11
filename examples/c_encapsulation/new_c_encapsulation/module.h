#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

// void (*load_function_context) (struct ModulePublic* this_instance);
// void (*unload_function_context) (void);
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

enum module_type {
    TYPE_MODULE_PUBLIC,
    TYPE_MODULE_PRIVATE,
};

void loadInterfaceModule(void);

struct ModulePublic* constructModuleV2Heap(int _foo);
struct ModulePublic* constructModule(int _foo);

void destroyModuleV2(struct ModulePublic* module_to_destroy_public);
void destroyModule(struct ModulePublic* module_to_destroy);

#endif // MODULE_H_DEFINED