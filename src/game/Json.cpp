#include <fstream>

#include <string.h>
#include <sys/stat.h>

#include "Json.hpp"
#include "Log.hpp"

cJSON* Json::readFromFile(std::string file_path)
{
    const int bytes = 1;
    const char* cjson_error_ptr;

    int ret;
    char* json_string;
    std::string msg;
    FILE* file;
    cJSON* json;
    struct stat file_stat;

    ret = 0;
    json = 0;
    file_stat = {};
    json_string = 0;

    ret = stat(file_path.c_str(), &file_stat);
    if (0 != ret) {
        msg = "Failed to stat ";
        msg += file_path;
        msg += " : ";
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }

    json_string = (char*) calloc(file_stat.st_size, bytes);
#if DEBUG_VERBOSE
    printf("%s size is: %ld\n", file_path.c_str(), file_stat.st_size);
#endif

    file = fopen(file_path.c_str(), "r");
    if (0 == file) {
        msg = "Failed to open file ";
        msg += file_path;
        msg += " : ";
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }

    ret = fread(json_string, bytes, file_stat.st_size, file);
    if (ret != file_stat.st_size) {
        free(json_string);
        json_string = 0;

        msg = "Read unexpected amount (";
        msg += ret;
        msg += ") of bytes from file ";
        msg += file_path;
        throw std::runtime_error(msg);
    }

    ret = fclose(file);
    if (0 != ret) {
        free(json_string);
        json_string = 0;

        msg = "Failed to close file ";
        msg += file_path;
        msg += " : ";
        msg += strerror(errno);
        throw std::runtime_error(msg);
    }
    file = 0;

    json = cJSON_Parse(json_string);
    if (nullptr == json) {
        free(json_string);
        json_string = 0;

        cjson_error_ptr = cJSON_GetErrorPtr();

        msg = "Failed to cJSON_Parse";
        if (nullptr == cjson_error_ptr) {
            throw std::runtime_error(msg);
        }

        msg += ": ";
        msg += cjson_error_ptr;
        throw std::runtime_error(msg);
    }

    free(json_string);
    json_string = 0;

    return json;
}
