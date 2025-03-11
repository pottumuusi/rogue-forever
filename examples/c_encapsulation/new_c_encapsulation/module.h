#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

struct ModulePublic {
    struct ModulePublic* this_instance; // TODO remove if unnecessary?

    void (*load_function_context) (struct ModulePublic* this_instance);
    void (*unload_function_context) (void);
    int (*add_to_foo) (int operand);
    int (*add_to_fooV2) (int operand);
};

struct ModulePrivate {
    struct ModulePublic* owner; // TODO remove this field

    int foo;
};

// Rename as `Module`
struct ModuleFull {
    struct ModulePublic public;
    struct ModulePrivate private;
};

enum module_type {
    TYPE_MODULE_PUBLIC,
    TYPE_MODULE_PRIVATE,
};

struct ModulePublic* constructModuleV2(int _foo);
struct ModulePublic* constructModule(int _foo);

void destroyModuleV2(struct ModulePublic* module_to_destroy_public);
void destroyModule(struct ModulePublic* module_to_destroy);

#endif // MODULE_H_DEFINED