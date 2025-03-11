/*
 * A module for providing utilities for module management.
 *
 * To have visibility to `this` and `this_private` of any module during
 * compilation, 'function context' function definitions are placed behind
 * macros.
 */

#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

#define MODULE_GENERATE_FUNCTION_CONTEXT_DECLARATIONS \
    static void unload_function_context(void);        \
    static void validate_function_context(void);

#define MODULE_GENERATE_UNLOAD_FUNCTION_CONTEXT \
    static void                                 \
    unload_function_context(void)               \
    {                                           \
        this = NULL;                            \
        this_private = NULL;                    \
    }

#define MODULE_GENERATE_VALIDATE_FUNCTION_CONTEXT \
    static void                                   \
    validate_function_context(void)               \
    {                                             \
        assert(NULL != this);                     \
        assert(NULL != this_private);             \
    }

#endif // MODULE_H_DEFINED