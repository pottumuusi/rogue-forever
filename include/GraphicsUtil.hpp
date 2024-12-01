#ifndef GRAPHICS_UTIL_HPP_DEFINED
#define GRAPHICS_UTIL_HPP_DEFINED

#include "Map.hpp"
#include "Tile.hpp"

class GraphicsUtil
{
public:
    static void generate_tiles_main(spritesheet_pool& spritesheet_pool, tile_pool& tile_pool);
    static void generate_tiles_player(Spritesheet& spritesheet_player, tile_pool& tile_pool_player);
    static void load_spritesheets_map(spritesheet_pool& spritesheetPool, Map& map);
    static void load_spritesheet_player(Spritesheet& spritesheet_player);
    static void generateTileIdMapFile(tile_id_map& tileIdMap);

private:
};

#endif // GRAPHICS_UTIL_HPP_DEFINED
