#ifndef JSON_HPP_DEFINED
#define JSON_HPP_DEFINED

#include <nlohmann/json.hpp> // TODO remove dependency when cJSON is in use
#include <cJSON.h>

#include <string>

class Json
{
public:
    static cJSON* readFromFile(std::string filename);
    static nlohmann::json readFromFileNlohmann(std::string filename);

private:
};

#endif // JSON_HPP_DEFINED
