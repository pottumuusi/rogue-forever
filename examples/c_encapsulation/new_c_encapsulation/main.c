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
 *
 * One solution is to have a public interface, which takes module struct
 * as a parameter, sets up this and calls an internal interface, which
 * is devoid of the struct.
 *
 * A simple solution is to have public functions in modules. The idea of always
 * exposing functions via fields of struct is to be scrapped. The module
 * containing a function can be detected from the struct passed as an argument.
 * The struct argument also protects from identical function signatures in
 * two different modules. A policy for designing is to always pass struct
 * pointer pointing to module data to a module function. If the function
 * does not need module data, then the function does not belong to the module.
 * This applies to modules which are instantiated more than once.
 *
 * It would seem to align with C design to separate the interface and data from
 * each other. So to provide functions which take module instances as
 * parameters and operate with their data. The instantiable module definition
 * should not contain any behavior, only data. The resulting call site could
 * look like the following:
 *
 * Module->add_to_foo(module_instance, 123);
 *
 * The signature of the function can be simplified (in an attempt to make it
 * easier to recall purpose of functions from minimal set of parameters) by
 * writing an internal function with reduced parameters, the instance being
 * passed in some other way, such as static global variable. And here the
 * information hiding comes into play. Private data of the instance is made
 * easily accessible in the internal function. When private data is implemented
 * as a field of ModuleFull and ModulePublic is returned from the constructor,
 * it is possible to access private data outside the module. Accessing private
 * data outside its module is against design principles. I deem this private
 * data hiding to be sufficient to prevent accidental use outside their modules.
 *
 * If it is wanted to prevent use of private data alltogether outside their
 * modules, return public instance pointer, have a static list of full module
 * instances containing pointer to both public and private instance and have a
 * function for finding private counterpart of the public instance by accessing
 * the list of full module instances. As a drawback, this should prevent the
 * possibility to find private counterpart for instances constructed with
 * automatic storage (stored to stack).
 * https://en.wikipedia.org/wiki/C_syntax#Storage_class_specifiers
 */

#include "module.h"

#include <stddef.h>
#include <stdio.h>

// Define:
// * DEBUG to be 1 in compiler flags for enabling debug features.
// * INVALID_ACCESS_TO_PRIVATE_DATA to be 1 for producing a compilation error.

extern struct ModuleInterface Module;

int main(void)
{
    struct ModulePublic* object_module;

    int result;

    result = 0;
    object_module = NULL;

#if 0
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
    object_test = NULL;
#endif
    loadInterfaceModule();

    object_module = constructModuleV2Heap(4);

    result = Module.add_to_fooV2(object_module, 44);
    printf("After calling add_to_foo, result is: %d\n", result);

#if INVALID_ACCESS_TO_PRIVATE_DATA
    printf("object_module->foo is: %d", object_module->foo);
#endif

    destroyModuleV2(object_module);
    object_module = NULL;

    return 0;
}