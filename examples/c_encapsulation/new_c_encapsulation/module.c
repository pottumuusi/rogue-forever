#include "module.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int add_to_fooV2(struct ModulePublic* object_public, int operand);
static int add_to_fooV2_internal(int operand);

static void load_function_contextV2(struct ModulePublic* new_this);
static void unload_function_context(void);
static void validate_function_context(void);

// Function context begin
static struct ModulePublic* this;
static struct ModulePrivate* this_private;
// Function context end

struct ModuleInterface Module;

void
loadInterfaceModule(void)
{
    Module.add_to_fooV2 = add_to_fooV2;
}

struct ModulePublic*
constructModuleV2Heap(int _foo)
{
    struct Module* object_full;
    struct ModulePublic* object_public;
    struct ModulePrivate* object_private;

    object_full = NULL;

    object_full = calloc(1, sizeof(struct Module));
    if (NULL == object_full) {
        fprintf(stderr, "Failed to allocate memory for full object\n");
        return NULL;
    }

    object_public = &(object_full->public);
    object_private = &(object_full->private);

    (void) object_public; // Here would initialize public data
    object_private->foo = _foo;

    printf("constructModuleV2Heap, object_full is: %p\n", object_full);
    printf("constructModuleV2Heap, object_public is: %p\n", object_public);
    printf("constructModuleV2Heap, object_private is: %p\n", object_private);

    if ((void*) object_full != (void*) object_public) {
        fprintf(stderr, "Unexpected memory address for public field of module\n");
        free(object_full);
        object_full = NULL;
        return NULL;
    }

    return object_public;
}

void
destroyModuleV2(struct ModulePublic* module_to_destroy_public)
{
    struct Module* module_to_destroy =
        (struct Module*) module_to_destroy_public;

    free(module_to_destroy);
}

static void
load_function_contextV2(struct ModulePublic* new_this)
{
    if (NULL != this || NULL != this_private) {
        fprintf(stderr, "Previous context has not been unloaded. Please report this bug.");
        exit(1);
    }

    this = new_this;
    this_private = &(((struct Module*) new_this)->private);
}

static void
unload_function_context(void)
{
    this = NULL;
    this_private = NULL;
}

// TODO check if validate_function_context can be swiftly moved to module.c
// when this file has been renamed to demo.c.
static void
validate_function_context(void)
{
    assert(NULL != this);
    assert(NULL != this_private);
}

static int
add_to_fooV2(struct ModulePublic* object_public, int operand)
{
    int result = 0;

    load_function_contextV2(object_public);

    result = add_to_fooV2_internal(operand);

    unload_function_context();

    return result;
}

static int
add_to_fooV2_internal(int operand)
{
    validate_function_context();

    return this_private->foo + operand;
}