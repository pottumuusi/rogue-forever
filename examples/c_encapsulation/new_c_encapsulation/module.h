#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

struct ModulePublic {
    struct ModulePublic* this_instance; // TODO remove if unnecessary?

    void (*load_function_context) (struct ModulePublic* this_instance);
    void (*unload_function_context) (void);
    int (*add_to_foo) (int operand);
};

struct ModulePrivate {
    struct ModulePublic* owner;

    int foo;
};

enum module_type {
    TYPE_MODULE_PUBLIC,
    TYPE_MODULE_PRIVATE,
};

struct ModulePublic* constructModule(int _foo);

void destroyModule(struct ModulePublic* module_to_destroy);

#endif // MODULE_H_DEFINED