/*
 * A utility module providing module management functions.
 */

#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

// TODO remove if not needed. There should not be a need for this enum at
// the next stage of implementation.
enum module_type {
    TYPE_MODULE_PUBLIC,
    TYPE_MODULE_PRIVATE,
};

static int append_to_public_objects(void* new_module);
static int append_to_private_objects(void* new_module);
static int append_to_objects(void* new_module, enum module_type type);
static int insert_to_objects(void* new_module, int insert_index, void** objects_array);
static struct ModulePrivate* find_matching_private_object(struct ModulePublic const * const public_object);

#if 0
static void destroy_object()
#endif

struct ModulePublic {


    // void (*load_function_context) (struct DemoPublic* this_instance);
    // void (*unload_function_context) (void);
    // int (*add_to_foo) (int operand);
};


#endif // MODULE_H_DEFINED
