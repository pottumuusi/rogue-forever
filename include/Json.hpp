#ifndef JSON_HPP_DEFINED
#define JSON_HPP_DEFINED

#include <cJSON.h>

#include <string>

struct InterfaceJson {
    void (*moduleCleanupJson) (void);
    cJSON* (*read_from_file) (std::string file_path);
};

void constructInterfaceJson(void);

#endif // JSON_HPP_DEFINED
