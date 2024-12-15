#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include "Json.hpp"

class Map
{
public:
    Map() = delete;
    Map(std::string path);

    cJSON* getTmj(void);
    nlohmann::json getTmjNlohmann(void);
    cJSON* get_layers_array(void);
    std::vector<nlohmann::json> getLayersNlohmann(void);
    int getLayerAmount(void);
    int getLayerAmountNlohmann(void);
    std::uint32_t get_tiled_gid(const int x, const int y, const int layerNum);
    std::uint32_t get_tiled_gid_nlohmann(const int x, const int y, const int layerNum);

private:
    void loadJson(std::string path);

    cJSON* mapTmj;
    nlohmann::json mapTmjNlohmann;
};

#endif // MAP_HPP_DEFINED
