/*
 * Scetching the idea of how to implement private data for modules.
 *
 * module_test_addToFoo(module_test);
 *
 * module_test.addToFoo();
 * module_test.addToFoo(module_test, 2);
 * module_test.foo; // This should produce an error.
 *
 * Having `module_test` twice in the statement looks clumsy. It would be
 * needed to write Module* as the first parameter of every function.
 *
 * ENTER_CONTEXT(module_test)
 * module_test.addToFoo();
 * EXIT_CONTEXT()
 *
 * How to assure exit for every enter? One way to assure is to grep and
 * check that there are as many enter calls as there are exit calls. Again
 * the repeated `module_test` looks clumsy. The repetition may look clumsy,
 * but it simultaneously acts as a sanity check. I.e. should enter
 * `module_test` context when proceeding to run `module_test` functions.
 *
 * INVOKE_MODULE_FUNCTION(
 *     module_test,
 *     addToFoo,
 *     2)
 * -> module_test.addToFoo(module_test, 2)
 *
 * Here it would be needed to write Module* as the first parameter of every
 * function.
 *
 * SET_MODULE(module_test);
 * SET_FUNCTION(addToFoo);
 * SET_ARGUMENTS(2);
 * INVOKE_MODULE_FUNCTION(); // Should finish by clearing the context.
 *
 * Pass the arguments using heap memory instead of giving to function
 * call arguments.
 *
 * The arguments should be cast to needed types in the invoked function.
 * How to ensure type safety? In this case the arguments need allocated
 * memory. How much should be allocated?
 */

#include "module.h"

#include <stddef.h>
#include <stdio.h>

// Define DEBUG to be 1 in compiler flags for enabling debug features.

int main(void)
{
    struct ModulePublic* object_test;

    int result;

    result = 0;
    object_test = NULL;

    object_test = constructModule(2);
    if (NULL == object_test) {
        fprintf(stderr, "Failed to construct module");
        return 1;
    }

    object_test->load_function_context(object_test);
    result = object_test->add_to_foo(22);
    object_test->unload_function_context();

    printf("After calling add_to_foo, result is: %d\n", result);

    destroyModule(object_test);

    return 0;
}