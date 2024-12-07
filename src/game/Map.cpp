#include "Log.hpp"
#include "Map.hpp"

Map::Map(std::string path)
{
    loadJson(path);
}

void Map::loadJson(std::string path)
{
    mapTmj = Json::readFromFile(path);
    // mapTmj = Json::readFromFileNlohmann(path);
}

cJSON* Map::getTmj(void)
{
    return mapTmj;
}

nlohmann::json Map::getTmjNlohmann(void)
{
    return mapTmjNlohmann;
}

// TODO make private
cJSON* Map::getLayers(void)
{
    cJSON* json_layers;

    json_layers = NULL;

    json_layers = cJSON_GetObjectItemCaseSensitive(mapTmj, "layers");

    if ( ! cJSON_IsArray(json_layers)) {
        throw std::runtime_error(
            "Read 'layers' from tmj, but did not get an array.");
    }

    return json_layers;
}

std::vector<nlohmann::json> Map::getLayersNlohmann(void)
{
    return mapTmjNlohmann["layers"];
}

int Map::getLayerAmount(void)
{
    cJSON* json_layers;

    json_layers = NULL;

    json_layers = getLayers();

    return cJSON_GetArraySize(json_layers);
}

int Map::getLayerAmountNlohmann(void)
{
    return getLayersNlohmann().size();
}

// TEST_F(TestMap, GetGidOfFirstLayer)
// {
//     const int layer = 0;
//     const int x = 9;
//     const int y = 7;
//     Map testMap = Map("test_data/map2_16x16_redone.tmj");
//     EXPECT_EQ(testMap.get_tiled_gid(x, y, layer), 347);
// }
std::uint32_t Map::get_tiled_gid(const int x, const int y, const int layer_num)
{
    std::uint32_t tiled_gid;
    int layer_width;
    int layer_height;
    bool out_of_bounds;

    cJSON* json_layers_array = NULL;
    cJSON* json_layer = NULL;
    std::vector<uint64_t> layer_data_array;

    tiled_gid = 0;
    layer_width = 0;
    layer_height = 0;
    out_of_bounds = true;   

    (void) x;
    (void) y;

    out_of_bounds = layer_num < 0 || layer_num >= getLayerAmount();
    if (out_of_bounds) {
        Log::w("Layer number out of bounds while getting tiled gid");
        return 0;
    }

    json_layers_array = getLayers();
    json_layer = cJSON_GetArrayItem(json_layers_array, layer_num);
    layer_data_array = 

    printf("get_tiled_gid not yet implemented\n");

    return -1;
}

std::uint32_t Map::get_tiled_gid_nlohmann(const int x, const int y, const int layer_num)
{
    std::uint32_t tiled_gid;
    int layer_width;
    int layer_height;
    bool out_of_bounds;

    tiled_gid = 0;
    layer_width = 0;
    layer_height = 0;
    out_of_bounds = true;

    out_of_bounds = layer_num < 0 || layer_num >= getLayerAmountNlohmann();
    if (out_of_bounds) {
        Log::w("Layer number out of bounds while getting tiled gid");
        return 0;
    }

    std::vector<nlohmann::json> layers_array = getLayersNlohmann();
    nlohmann::json layer = layers_array[layer_num];
    std::vector<uint64_t> layer_data_array = layer["data"];

    layer_width = layer["width"];
    layer_height = layer["height"];

    out_of_bounds =
        x < 0 || x > (layer_width - 1) ||
        y < 0 || y > (layer_height - 1);
    if (out_of_bounds) {
        return 0;
    }

    tiled_gid = layer_data_array[(layer_width * y) + x];

    return tiled_gid;
}
