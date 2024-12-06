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

nlohmann::json Spritesheet::get_json(void)
{
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
    json = Json::readFromFileNlohmann(pathJson);
}

void Spritesheet::fetch_firstgid(Map& map)
{
    int tilesets_array_size;

    std::string tileset_source_str;

    nlohmann::json tmj;
    nlohmann::json tileset;

    const std::string spritesheet_name = get_name();

    tilesets_array_size = -1;
    tileset_source_str = "N/A";

    tmj = map.getTmj();

    if ( ! tmj.is_object()) {
        std::string msg = "While fetching firstgid, top level map .tmj ";
        msg += "JSON value is not an object";
        throw std::runtime_error(msg);
    }

    auto tilesets_array = tmj["tilesets"];
    tilesets_array_size = tmj["tilesets"].size();

    for (int i = 0; i < tilesets_array_size; ++i) {
        tileset = tilesets_array[i];
        tileset_source_str = tileset["source"];

        if (0 != tileset_source_str.compare(spritesheet_name + ".tsx")) {
            continue;
        }

        auto tileset_firstgid = tileset["firstgid"];

        set_tiled_firstgid(tileset_firstgid);

        return;
    }

    throw std::runtime_error("Could not find firstgid for spritesheet: " + name);
}
