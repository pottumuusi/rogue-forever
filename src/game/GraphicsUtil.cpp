#include "GraphicsUtil.hpp"

#include <array>
#include <filesystem>
#include <fstream>

#include "Log.hpp"

/*
 * Generate tiles which are considered to be part of map. Currently this
 * includes monsters as well. Monsters are added to a Tiled map, as any
 * other map components, such as trees and walls. Monsters are currently
 * not interactive and there is no mechanism to map behavior attributes
 * to any tiles.
 *
 * Use GID to index a tile. Tiles are created from all given spritesheets.
 * GID numbers of different spritesheets do not overlap, so all tiles are
 * inserted to a single map (data structure), `tile_pool`.
 *
 * TODO check if this function is tested and test if feasible:
 * https://github.com/pottumuusi/rogue-forever/issues/24
 */
void GraphicsUtil::generate_tiles_map(
    spritesheet_pool& spritesheet_pool,
    tile_pool& tile_pool)
{
    int spritesheet_tile_height;
    int spritesheet_tile_width;
    int spritesheet_width;

    unsigned int tiled_id;
    unsigned int tiled_gid;

    cJSON* json;
    cJSON* json_tmj;
    cJSON* json_data_item;
    cJSON* json_data_array;
    cJSON* json_layers_array;

    std::shared_ptr<SDL_Texture> texture_spritesheet;

    spritesheet_tile_height = -1;
    spritesheet_tile_width = -1;
    spritesheet_width = -1;

    tiled_id = 0;
    tiled_gid = 0;

    json = NULL;
    json_tmj = NULL;
    json_data_item = NULL;
    json_data_array = NULL;
    json_layers_array = NULL;

#if DEBUG
    Log::d("clearing tile_pool");
#endif // DEBUG
    tile_pool.clear();

    for (Spritesheet& spritesheet : spritesheet_pool) {
        json_tmj = spritesheet.get_json();
        texture_spritesheet = spritesheet.get_texture();

#if DEBUG_VERBOSE
        Log::d("getting tile height and width");
#endif // DEBUG

        json = cJSON_GetObjectItemCaseSensitive(json_tmj, "tileheight");
        spritesheet_tile_height = json->valueint;

        json = cJSON_GetObjectItemCaseSensitive(json_tmj, "tilewidth");
        spritesheet_tile_width = json->valueint;

        if (32 != spritesheet_tile_height || 32 != spritesheet_tile_width) {
            throw std::runtime_error("Spritesheet tile size is not 32x32");
        }

        json_layers_array = cJSON_GetObjectItemCaseSensitive(
            json_tmj, "layers");
        if (1 != cJSON_GetArraySize(json_layers_array)) {
            throw std::runtime_error(
                "Spritesheet JSON does not contain 1 layer");
        }

        json = cJSON_GetArrayItem(json_layers_array, 0);
        json_data_array = cJSON_GetObjectItemCaseSensitive(json, "data");

        json = cJSON_GetObjectItemCaseSensitive(json, "width");
        spritesheet_width = json->valueint;

        cJSON_ArrayForEach(json_data_item, json_data_array) {
            if ( ! cJSON_IsNumber(json_data_item)) {
                throw std::runtime_error("Data array item is not a number.");
            }

            // ID for spritesheet and GID for whole map
            tiled_id = json_data_item->valueint;
            tiled_gid = spritesheet.get_tiled_firstgid() - 1 + tiled_id;

            if (0 == tiled_id) {
                break;
            }

            if (0 != (tiled_gid & g_constants::TILED_FLIPPED_HORIZONTALLY_FLAG)) {
                throw std::runtime_error("Horizontally flipped tiles not supported");
            }

            if (0 != (tiled_gid & g_constants::TILED_FLIPPED_VERTICALLY_FLAG)) {
                throw std::runtime_error("Vertically flipped tiles not supported");
            }

            if (0 != (tiled_gid & g_constants::TILED_FLIPPED_DIAGONALLY_FLAG)) {
                throw std::runtime_error("Diagonally flipped tiles not supported");
            }

            if (0 != (tiled_gid & g_constants::TILED_ROTATED_HEXAGONAL_120_FLAG)) {
                throw std::runtime_error("Rotated tiles not supported");
            }

            const int x = (tiled_id - 1) % spritesheet_width;
            const int y = (tiled_id - 1) / spritesheet_width;

            tile_pool[tiled_gid] = Tile(
                        "namePlaceholder",
                        texture_spritesheet,
                        x * g_constants::TILE_WIDTH,
                        y * g_constants::TILE_HEIGHT,
                        g_constants::TILE_WIDTH,
                        g_constants::TILE_HEIGHT,
                        tiled_gid,
                        tiled_id);
        }
    }
}

/*
 * TODO write a test if feasible:
 * https://github.com/pottumuusi/rogue-forever/issues/24
 */
void GraphicsUtil::generate_tiles_player(Spritesheet& spritesheet_player, tile_pool& tile_pool_player)
{
    int spritesheet_tile_height;
    int spritesheet_tile_width;
    int spritesheet_width;

    unsigned int tiled_id;

    cJSON* json;
    cJSON* json_tmj;
    cJSON* json_data_item;
    cJSON* json_data_array;
    cJSON* json_layers_array;

    std::shared_ptr<SDL_Texture> texture_spritesheet;

    spritesheet_tile_height = -1;
    spritesheet_tile_width = -1;
    spritesheet_width = -1;

    tiled_id = 0;

    json = NULL;
    json_tmj = NULL;
    json_data_item = NULL;
    json_data_array = NULL;
    json_layers_array = NULL;

    json_tmj = spritesheet_player.get_json();
    texture_spritesheet = spritesheet_player.get_texture();

    json = cJSON_GetObjectItemCaseSensitive(json_tmj, "tileheight");
    spritesheet_tile_height = json->valueint;

    json = cJSON_GetObjectItemCaseSensitive(json_tmj, "tilewidth");
    spritesheet_tile_width = json->valueint;

    if (32 != spritesheet_tile_height || 32 != spritesheet_tile_width) {
        throw std::runtime_error("Player spritesheet tile size is not 32x32");
    }

    json_layers_array = cJSON_GetObjectItemCaseSensitive(json_tmj, "layers");
    if ( ! cJSON_IsArray(json_layers_array)) {
        throw std::runtime_error(
            "'layers' of player spritesheet JSON is not an array");
    }
    if (1 != cJSON_GetArraySize(json_layers_array)) {
        throw std::runtime_error("Player spritesheet JSON does not contain 1 layer");
    }

    json = cJSON_GetArrayItem(json_layers_array, 0);
    json_data_array = cJSON_GetObjectItemCaseSensitive(json, "data");

    json = cJSON_GetObjectItemCaseSensitive(json, "width");
    spritesheet_width = json->valueint;

    cJSON_ArrayForEach(json_data_item, json_data_array) {
        if ( ! cJSON_IsNumber(json_data_item)) {
            throw std::runtime_error("Data array item is not a number.");
        }

        tiled_id = json_data_item->valueint;

        if (0 == tiled_id) {
            break;
        }

        const int x = (tiled_id - 1) % spritesheet_width;
        const int y = (tiled_id - 1) / spritesheet_width;

        tile_pool_player[tiled_id] = Tile(
            "namePlaceholder",
            texture_spritesheet,
            x * g_constants::TILE_WIDTH,
            y * g_constants::TILE_HEIGHT,
            g_constants::TILE_WIDTH,
            g_constants::TILE_HEIGHT,
            tiled_id
        );
    }
}

/*
 * Load all spritesheets whose components are included in a map. When
 * components of a spritesheet are used in a map, Tiled adds an entry
 * to `tilesets` list of map tmj files.
 *
 * TODO write a test if feasible:
 * https://github.com/pottumuusi/rogue-forever/issues/24
 */
void GraphicsUtil::load_spritesheets_map(
    spritesheet_pool& spritesheet_pool,
    Map& map)
{
    cJSON* json;
    cJSON* json_tmj;
    cJSON* json_tileset;
    cJSON* json_tilesets_array;

    std::string tileset_source;
    std::vector<std::string> tileset_names;

    unsigned int json_tilesets_size;

    json = NULL;
    json_tmj = NULL;
    json_tileset = NULL;
    json_tilesets_array = NULL;
    json_tilesets_size = 0;

    json_tmj = map.getTmj();
    if ( ! cJSON_IsObject(json_tmj)) {
        throw std::runtime_error(
            "Top level map .tmj JSON value is not an object");
    }

    json_tilesets_array = cJSON_GetObjectItemCaseSensitive(json_tmj, "tilesets");
    if ( ! cJSON_IsArray(json_tilesets_array)) {
        throw std::runtime_error("'tilesets' of tmj is not an array");
    }

    json_tilesets_size = cJSON_GetArraySize(json_tilesets_array);
    for (unsigned int i = 0; i < json_tilesets_size; i++) {
        json_tileset = cJSON_GetArrayItem(json_tilesets_array, i);
        if ( ! cJSON_IsObject(json_tileset)) {
            throw std::runtime_error("tileset is not an object");
        }

        json = cJSON_GetObjectItemCaseSensitive(json_tileset, "source");
        if ( ! cJSON_IsString(json)) {
            throw std::runtime_error("Tileset source is not a string");
        }

        tileset_source = json->valuestring;
        tileset_names.push_back(tileset_source);
    }

    for (std::string spritesheet_name : Spritesheet::spritesheet_names) {
        for (std::string tileset_name : tileset_names) {
            if (0 == tileset_name.compare(spritesheet_name + ".tsx")) {
                spritesheet_pool.push_back(Spritesheet(spritesheet_name, map));
                break;
            }
        }
    }
}

void GraphicsUtil::load_spritesheet_player(Spritesheet& spritesheet_player)
{
    spritesheet_player = Spritesheet(Spritesheet::spritesheet_name_player);
}

/* Keeping as a reference for using std::filesystem
 *
 * TODO remove when the project is written in C.
 * https://github.com/pottumuusi/rogue-forever/issues/30
 */
void GraphicsUtil::generateTileIdMapFile(tile_id_map& tileIdMap)
{
    int currentId = 0;
    const std::string tileIdMapFileName = "generated/tile_id_map.txt";
    std::ofstream outStream;
    std::string fileContent;

    const std::filesystem::path dirGenerated = {"generated"};

    if (!std::filesystem::exists(dirGenerated)) {
        std::filesystem::create_directory(dirGenerated);
    }

    outStream.open(tileIdMapFileName, std::ios::out);
    if (outStream.fail()) {
        throw std::runtime_error("Failed to open file: " + tileIdMapFileName + " for writing");
    }

    for (std::string tileName : tileIdMap) {
        fileContent += std::to_string(currentId);
        fileContent += ": ";
        fileContent += tileName;
        fileContent += "\n";
        currentId++;
    }

    outStream.write(fileContent.c_str(), fileContent.size());
}
