#include "Log.hpp"
#include "Map.hpp"

Map::Map(std::string path)
{
    loadJson(path);
}

void Map::loadJson(std::string path)
{
    mapTmj = Json::readFromFile(path);
}

cJSON* Map::getTmj(void)
{
    return mapTmj;
}

// TODO make private
cJSON* Map::get_layers_array(void)
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

int Map::getLayerAmount(void)
{
    cJSON* json_layers;

    json_layers = NULL;

    json_layers = get_layers_array();

    return cJSON_GetArraySize(json_layers);
}

std::uint32_t Map::get_tiled_gid(const int x, const int y, const int layer_num)
{
    std::uint32_t tiled_gid;
    int layer_width;
    int layer_height;
    bool out_of_bounds;

    cJSON* json;
    cJSON* json_layers_array;
    cJSON* json_layer;
    cJSON* json_layer_data_array;

    json = NULL;
    json_layers_array = NULL;
    json_layer = NULL;
    json_layer_data_array = NULL;
    tiled_gid = 0;
    layer_width = 0;
    layer_height = 0;
    out_of_bounds = true;

    out_of_bounds = layer_num < 0 || layer_num >= getLayerAmount();
    if (out_of_bounds) {
        Log::w("Layer number out of bounds while getting tiled gid");
        return 0;
    }

    json_layers_array = get_layers_array();

    json_layer = cJSON_GetArrayItem(json_layers_array, layer_num);
    if ( ! cJSON_IsObject(json_layer)) {
        throw std::runtime_error(
            "json_layer read from json_layers_array is not an object");
    }

    json_layer_data_array =
        cJSON_GetObjectItemCaseSensitive(json_layer, "data");
    if ( ! cJSON_IsArray(json_layer_data_array)) {
        throw std::runtime_error(
            "json_layer_data_array read from json_layer is not an array");
    }

    json = cJSON_GetObjectItemCaseSensitive(json_layer, "width");
    json = cJSON_GetObjectItemCaseSensitive(json_layer, "height");

    layer_width = json->valueint;
    layer_height = json->valueint;

    out_of_bounds =
        x < 0 || x > (layer_width - 1) ||
        y < 0 || y > (layer_height - 1);
    if (out_of_bounds) {
        return 0;
    }

    json = cJSON_GetArrayItem(json_layer_data_array, (layer_width * y) + x);
    tiled_gid = json->valueint;

    return tiled_gid;
}
