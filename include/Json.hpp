#ifndef JSON_HPP_DEFINED
#define JSON_HPP_DEFINED

#include <cJSON.h>

#include <string>

class Json
{
public:
    static cJSON* readFromFile(std::string filename);

private:
};

#endif // JSON_HPP_DEFINED
