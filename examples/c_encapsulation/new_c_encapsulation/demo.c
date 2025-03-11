#include "demo.h"
#include "module.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int add_to_foo(struct Demo* object_public, int operand);
static int add_to_foo_internal(int operand);

static void load_function_context(struct Demo* new_this);

MODULE_GENERATE_FUNCTION_CONTEXT_DECLARATIONS

// Function context begin
static struct Demo* this;
static struct DemoPrivate* this_private;
// Function context end

struct DemoInterface Demo;

void
loadInterfaceDemo(void)
{
    Demo.add_to_foo = add_to_foo;
}

struct Demo*
constructDemoHeap(int _foo)
{
    struct DemoFull* object_full;
    struct Demo* object_public;
    struct DemoPrivate* object_private;

    object_full = NULL;

    object_full = calloc(1, sizeof(struct DemoFull));
    if (NULL == object_full) {
        fprintf(stderr, "Failed to allocate memory for full object\n");
        return NULL;
    }

    object_public = &(object_full->public);
    object_private = &(object_full->private);

    (void) object_public; // Here would initialize public data
    object_private->foo = _foo;

    printf("constructDemoHeap, object_full is: %p\n", object_full);
    printf("constructDemoHeap, object_public is: %p\n", object_public);
    printf("constructDemoHeap, object_private is: %p\n", object_private);

    if ((void*) object_full != (void*) object_public) {
        fprintf(stderr, "Unexpected memory address for public field of module\n");
        free(object_full);
        object_full = NULL;
        return NULL;
    }

    return object_public;
}

void
destroyDemo(struct Demo* demo_to_destroy_public)
{
    struct DemoFull* demo_to_destroy =
        (struct DemoFull*) demo_to_destroy_public;

    free(demo_to_destroy);
}

static void
load_function_context(struct Demo* new_this)
{
    if (NULL != this || NULL != this_private) {
        fprintf(stderr, "Previous context has not been unloaded. Please report this bug.");
        exit(1);
    }

    this = new_this;
    this_private = &(((struct DemoFull*) new_this)->private);
}

MODULE_GENERATE_UNLOAD_FUNCTION_CONTEXT

MODULE_GENERATE_VALIDATE_FUNCTION_CONTEXT

static int
add_to_foo(struct Demo* object_public, int operand)
{
    int result = 0;

    load_function_context(object_public);

    result = add_to_foo_internal(operand);

    unload_function_context();

    return result;
}

static int
add_to_foo_internal(int operand)
{
    validate_function_context();

    return this_private->foo + operand;
}