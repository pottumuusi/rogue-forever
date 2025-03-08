#include "module.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define OBJECTS_SIZE 8

static int add_to_foo(int operand);
static int append_to_public_objects(void* new_module);
static int append_to_private_objects(void* new_module);
static int insert_to_objects(void* new_module, int insert_index, void** objects_array);
static int append_to_objects(void* new_module, enum module_type type);

static void load_function_context(struct ModulePublic* new_this);
static void unload_function_context(void);
static void validate_function_context(void);

static struct ModulePublic* objects_public[OBJECTS_SIZE] = {0};
static struct ModulePrivate* objects_private[OBJECTS_SIZE] = {0};

// Function context begin
static struct ModulePublic* this;
static struct ModulePrivate* this_private;
// Function context end

struct ModulePublic* constructModule(int _foo)
{
    int ret;

    struct ModulePublic* object_public;
    struct ModulePrivate* object_private;

    this = NULL;

    ret = 0;
    object_public = NULL;
    object_private = NULL;

    object_public = (struct ModulePublic*) calloc(1, sizeof(struct ModulePublic));
    if (NULL == object_public) {
        fprintf(stderr, "Failed to allocate memory for public object\n");
        return NULL;
    }

    object_public->this_instance = object_public;
    object_public->load_function_context = load_function_context;
    object_public->unload_function_context = unload_function_context;
    object_public->add_to_foo = add_to_foo;

#if DEBUG
    printf("Appending object_public with value: %p\n", object_public);
#endif
    ret = append_to_public_objects(object_public);
    if (0 != ret) {
        fprintf(stderr, "Failed to append object to public objects\n");
        return NULL;
    }

    object_private = (struct ModulePrivate*) calloc(1, sizeof(struct ModulePrivate));
    if (NULL == object_private) {
        fprintf(stderr, "Failed to allocate memory for private object\n");
        return NULL;
    }

    object_private->owner = object_public;
    object_private->foo = _foo;

#if DEBUG
    printf("Appending object_private with value: %p\n", object_private);
#endif
    ret = append_to_private_objects(object_private);
    if (0 != ret) {
        fprintf(stderr, "Failed to append object to private objects\n");
        return NULL;
    }

#if DEBUG
    printf("The first element of objects_public is: %p\n", objects_public[0]);
    printf("The first element of objects_private is: %p\n", objects_private[0]);
#endif

    return object_public;
}

void destroyModule(struct ModulePublic* module_to_destroy)
{
    for (int i = 0; i < sizeof objects_public; i++) {
        if (module_to_destroy == objects_public[i]) {
            free(module_to_destroy);
            objects_public[i] = NULL;
            return;
        }
    }

#if DEBUG
    fprintf(
        stderr,
        "Did not find module %p from public objects\n",
        module_to_destroy);
#endif // DEBUG
}

static struct ModulePrivate*
find_matching_private_object(struct ModulePublic const * const public_object)
{
    for (int i = 0; i < OBJECTS_SIZE; i++) {
        if (NULL == objects_private[i]) {
            continue;
        }

        if (public_object == objects_private[i]->owner) {
            return objects_private[i];
        }
    }

    // TODO dump information which may help in debugging
    fprintf(stderr, "Failed to find private object matching a public object. Please report this bug.");
    exit(1);
}

static void
load_function_context(struct ModulePublic* new_this)
{
    if (NULL != this || NULL != this_private) {
        // TODO dump information which may help in debugging
        fprintf(stderr, "Previous context has not been unloaded. Please report this bug.");
        exit(1);
    }

    this = new_this;
    this_private = find_matching_private_object(this);
}

static void
unload_function_context(void)
{
    this = NULL;
}

static void
validate_function_context(void)
{
    assert(NULL != this);
    assert(NULL != this_private);
}

static int
append_to_public_objects(void* new_module)
{
    append_to_objects(new_module, TYPE_MODULE_PUBLIC);
}

static int
append_to_private_objects(void* new_module)
{
    append_to_objects(new_module, TYPE_MODULE_PRIVATE);
}

static int
append_to_objects(void* new_module, enum module_type type)
{
    const int objects_public_size =
        sizeof(objects_public) / sizeof(objects_public[0]);
    const int objects_private_size =
        sizeof(objects_private) / sizeof(objects_private[0]);

    void** objects_array;

    objects_array = NULL;

    if (TYPE_MODULE_PRIVATE == type) {
        assert(OBJECTS_SIZE == objects_private_size);
        objects_array = (void**) objects_private;
    } else if (TYPE_MODULE_PUBLIC == type) {
        assert(OBJECTS_SIZE == objects_public_size);
        objects_array = (void**) objects_public;
    } else {
        fprintf(stderr, "Unknown module type: %d\n", type);
        return -1;
    }

    printf("sizeof objects_public[0] is: %d\n", sizeof objects_public[0]);
    printf("sizeof objects_public is: %d\n", sizeof objects_public);

    for (int i = 0; i < OBJECTS_SIZE; i++) {
        if (0 == objects_array[i]) {
            return insert_to_objects(new_module, i, objects_array);
        }
    }

    return -1;
}

static int
insert_to_objects(
    void* new_module,
    int insert_index,
    void** objects_array)
{
    if (insert_index < 0 || insert_index >= OBJECTS_SIZE) {
        fprintf(stderr, "Index out of bounds while inserting to objects\n");
        return -1;
    }

    if (0 != objects_array[insert_index]) {
#if DEBUG
        fprintf(stderr, "Refraining from inserting to already occupied public object slot");
#endif // DEBUG
        return -1;
    }

    objects_array[insert_index] = new_module;

    return 0;
}

static int add_to_foo(int operand)
{
    validate_function_context();

    return this_private->foo + operand;
}