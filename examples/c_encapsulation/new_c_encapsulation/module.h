/*
 * A module for providing utilities for module management.
 *
 * To have visibility to `this` and `this_private` of any module during
 * compilation, 'function context' function definitions are placed behind
 * macros.
 *
 * TODO write what is expected from the file that generates functions using the
 * macros defined here.
 */

#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

// TODO Have a single macro for generating all context functions.

// TODO consider renaming function_context to module_context

#define MODULE_GENERATE_DECLARATIONS           \
    static void module_unload_context(void);   \
    static void module_validate_context(void);

// TODO rename unload_function_context -> module_unload_context
#define MODULE_GENERATE_UNLOAD_CONTEXT \
static void                            \
module_unload_context(void)            \
{                                      \
    this_public = NULL;                \
    this_private = NULL;               \
}

#define MODULE_GENERATE_VALIDATE_CONTEXT \
static void                              \
module_validate_context(void)            \
{                                        \
    assert(NULL != this_public);         \
    assert(NULL != this_private);        \
}

#endif // MODULE_H_DEFINED
