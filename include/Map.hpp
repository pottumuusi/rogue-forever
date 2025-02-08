#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include <cstdint>

#include "Json.hpp"

class Map
{
public:
    Map() = delete;
    Map(std::string path);

    cJSON* getTmj(void);
    cJSON* get_layers_array(void);
    int getLayerAmount(void);
    std::uint32_t get_tiled_gid(const int x, const int y, const int layerNum);

private:
    void loadJson(std::string path);

    cJSON* mapTmj;
};

#endif // MAP_HPP_DEFINED
