#include <array>
#include <stdexcept>

#include "Assets.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"

const std::string Spritesheet::spritesheet_name_player = "player-collection-spritesheet";

const std::array<std::string, SPRITESHEET_POOL_SIZE> Spritesheet::spritesheet_names = {
    "dc-dngn-collection-spritesheet",
    "dc-misc-collection-spritesheet",
    "dc-mon-collection-spritesheet",
    "effect-collection-spritesheet",
    "gui-collection-spritesheet",
    "item-collection-spritesheet",
    Spritesheet::spritesheet_name_player,
    "spells-collection-spritesheet",
};

#if GRAPHICS_ENABLED
Spritesheet::Spritesheet(std::string _name)
    : name {_name}
{
    load_texture(Assets::spritesheetsDir + "/" + name + ".png");
    load_json(Assets::spritesheetsDir + "/" + name + ".tmj");
    tiledFirstgid = -1;
}
Spritesheet::Spritesheet(std::string _name, Map& map)
	: name {_name}
{
    load_texture(Assets::spritesheetsDir + "/" + name + ".png");
    load_json(Assets::spritesheetsDir + "/" + name + ".tmj");
    fetch_firstgid(map);
}
#elif BUILD_FOR_TESTS
Spritesheet::Spritesheet(std::string _name)
    : name {_name}
{
    load_json(Assets::testDataDir + "/" + name + ".tmj");
    tiledFirstgid = -1;
}
Spritesheet::Spritesheet(std::string _name, Map& map)
	: name {_name}
{
    load_json(Assets::testDataDir + "/" + name + ".tmj");
    fetch_firstgid(map);
}
#else
#error "Unknown build type"
#endif

std::string Spritesheet::get_name(void) const
{
    return name;
}

std::shared_ptr<SDL_Texture> Spritesheet::get_texture(void)
{
    return texture;
}

cJSON* Spritesheet::get_json(void)
{
    if ( ! cJSON_IsObject(json)) {
        throw std::runtime_error(
            "Top level spritesheet JSON value is not an object");
    }

    return json;
}

int Spritesheet::get_tiled_firstgid(void) const
{
    if (-1 == tiledFirstgid) {
        throw std::runtime_error("Tiled first GID not set for spritesheet");
    }

    return tiledFirstgid;
}

void Spritesheet::set_tiled_firstgid(int new_tiled_firstgid)
{
    tiledFirstgid = new_tiled_firstgid;
}

void Spritesheet::load_texture(std::string pathImage)
{
    Sdlw& sdlw = Sdlw::getReference();

    texture = sdlw.imgLoadTextureShared(pathImage);
    if (NULL == texture) {
        throw std::runtime_error("Failed to load texture: " + pathImage);
    }
}

void Spritesheet::load_json(std::string pathJson)
{
    json = Json::readFromFile(pathJson);
}

void Spritesheet::fetch_firstgid(Map& map)
{
    unsigned int tilesets_array_size;
    int tileset_firstgid;

    cJSON* json;
    cJSON* json_tmj;
    cJSON* json_tileset;
    cJSON* json_tilesets_array;

    std::string spritesheet_name;
    std::string tileset_source_str;

    json = NULL;
    json_tmj = NULL;
    json_tileset = NULL;
    json_tilesets_array = NULL;
    tilesets_array_size = 0;
    tileset_source_str = "N/A";
    spritesheet_name = "N/A";

    spritesheet_name = get_name();

    json_tmj = map.getTmj();
    if ( ! cJSON_IsObject(json_tmj)) {
        throw std::runtime_error(
            "While fetching firstgid, top level map .tmj JSON value is not an object");
    }

    json_tilesets_array = cJSON_GetObjectItemCaseSensitive(
        json_tmj, "tilesets");
    if ( ! cJSON_IsArray(json_tilesets_array)) {
        throw std::runtime_error("'tilesets' of tmj is not an array");
    }

    tilesets_array_size = cJSON_GetArraySize(json_tilesets_array);

    for (unsigned int i = 0; i < tilesets_array_size; i++) {
        json_tileset = cJSON_GetArrayItem(json_tilesets_array, i);
        if ( ! cJSON_IsObject(json_tileset)) {
            throw std::runtime_error("tilesets array element is not an object");
        }

        json = cJSON_GetObjectItemCaseSensitive(json_tileset, "source");
        tileset_source_str = json->valuestring;

        if (0 != tileset_source_str.compare(spritesheet_name + ".tsx")) {
            continue;
        }

        json = cJSON_GetObjectItemCaseSensitive(json_tileset, "firstgid");
        tileset_firstgid = json->valueint;

        set_tiled_firstgid(tileset_firstgid);

        return;
    }

    throw std::runtime_error(
        "Could not find firstgid for spritesheet: " + name);
}
