// TODO write constructor and define single global instance for calling
// functions.

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

// TODO pass objects_private or objects_public as a parameter
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

// TODO maybe have the new Module struct, which contains both Public and Private
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
destroy_object()
{
    //
}
