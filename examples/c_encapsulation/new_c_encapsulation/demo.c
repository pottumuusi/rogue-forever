#include "demo.h"

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

static void load_function_context(struct DemoPublic* new_this);
static void unload_function_context(void);
static void validate_function_context(void);

static struct DemoPublic* objects_public[OBJECTS_SIZE] = {0};
static struct DemoPrivate* objects_private[OBJECTS_SIZE] = {0};

// TODO consider wrapping public and private inside a Module struct. The
// Module struct could not be exposed as it would expose private portion
// of module. If Module struct is used inside a compilation unit, does it
// complicate or simplify code?
//
// Function context begin
static struct DemoPublic* this;
static struct DemoPrivate* this_private;
// Function context end

struct DemoPublic* constructDemo(int _foo)
{
    int ret;

    struct DemoPublic* object_public;
    struct DemoPrivate* object_private;

    this = NULL;

    ret = 0;
    object_public = NULL;
    object_private = NULL;

    object_public = (struct DemoPublic*) calloc(1, sizeof(struct DemoPublic));
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

    object_private = (struct DemoPrivate*) calloc(1, sizeof(struct DemoPrivate));
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

/*
 * Designed to be used for cleaning up what was created by constructDemo().
 */
void destroyDemo(struct DemoPublic* module_to_destroy)
{
    // TODO utilize module.c functions when destroying object
    for (int i = 0; i < sizeof objects_public; i++) {
        if (module_to_destroy == objects_public[i]) {
            free(module_to_destroy);
            objects_public[i] = NULL;
            return;
        }
    }

    // TODO destroy the coupled private object

#if DEBUG
    fprintf(
        stderr,
        "Did not find module %p from public objects\n",
        module_to_destroy);
#endif // DEBUG
}

static void
load_function_context(struct DemoPublic* new_this)
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

static int add_to_foo(int operand)
{
    validate_function_context();

    return this_private->foo + operand;
}
