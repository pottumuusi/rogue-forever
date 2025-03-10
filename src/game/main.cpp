#include <stdio.h>

#include <array>
#include <iostream>
#include <string>

#include "Assets.hpp"
#include "Constants.hpp"
#include "GraphicsUtil.hpp"
#include "Json.hpp"
#include "Log.hpp"
#include "Map.hpp"
#include "Sdlw.hpp"
#include "Spritesheet.hpp"
#include "Tile.hpp"

using screen_tiles = std::array<std::array<Tile*, g_constants::TILES_HORIZONTAL>, g_constants::TILES_VERTICAL>;
using screen_tiles_layers = std::vector<screen_tiles>;
using texture_pool = std::array<SDL_Texture*, g_constants::TEXTURE_POOL_SIZE>;

#if 0
void mainLoop(void) {
}
#endif

void renderTile(
        Sdlw& sdlw,
        Tile* tile,
        int screenX,
        int screenY)
{
    SDL_Rect srcRect = {
        tile->getSheetX(),
        tile->getSheetY(),
        tile->getSheetW(),
        tile->getSheetH(),
    };
    SDL_Rect dstRect = {
        screenX,
        screenY,
        tile->getSheetW(),
        tile->getSheetH()
    };

#if DEBUG_VERBOSE
    Log::d("Dumping tile which is getting rendered:");
    std::string msg = tile->getDump();
    Log::d(msg);
#endif

    sdlw.renderCopy(tile->getSheetTexture(), &srcRect, &dstRect);
}

void fillScreenTiles(
        tile_pool& tilePool,
        Map& map,
        int cameraX,
        int cameraY,
        screen_tiles_layers& screenTilesLayers)
{
    screen_tiles screenTiles;

    const int screenTilesColumnSize = screenTiles.size();
    const int screenTilesRowSize = screenTiles[0].size();

    // Camera is considered to be in the center of the screen. Shift half a
    // screen left and up and eventually draw level from there.
    int levelY = cameraY - (g_constants::TILES_VERTICAL / 2);
    int levelX = cameraX - (g_constants::TILES_HORIZONTAL / 2);
    int levelYEnd = levelY + screenTilesColumnSize;
    int levelXEnd = levelX + screenTilesRowSize;

    const int layerAmount = map.getLayerAmount();

    screenTilesLayers.clear();

    if (levelYEnd > screenTilesColumnSize) {
        levelYEnd = screenTilesColumnSize;
    }

    if (levelXEnd > screenTilesRowSize) {
        levelXEnd = screenTilesRowSize;
    }

    for (int layerNum = 0; layerNum < layerAmount; ++layerNum) {
        for (auto& tilesRow : screenTiles) {
            tilesRow.fill(0);
        }

        levelY = cameraY - (g_constants::TILES_VERTICAL / 2);

        for (int screenTileY = 0; screenTileY < screenTilesColumnSize; ++screenTileY) {
            if (levelY < 0) {
                ++levelY;
                continue;
            }

            if (levelY >= levelYEnd) {
                break;
            }

            levelX = cameraX - (g_constants::TILES_HORIZONTAL / 2);

            for (int screenTileX = 0; screenTileX < screenTilesRowSize; ++screenTileX) {
                if (levelX < 0) {
                    ++levelX;
                    continue;
                }

                if (levelX >= levelXEnd) {
                    break;
                }

                std::uint32_t tiledGid = map.get_tiled_gid(levelX, levelY, layerNum);
                // Leave as null if gid zero
                if (0 != tiledGid) {
                    screenTiles[screenTileY][screenTileX] = &(tilePool[tiledGid]);
                }

                ++levelX;
            }

            ++levelY;
        }

        screenTilesLayers.push_back(screenTiles);
    }
}

void renderScreenTiles(
        Sdlw& sdlw,
        const screen_tiles& screenTiles)
{
    for (size_t screenY = 0; screenY < screenTiles.size(); screenY++) {
        for (size_t screenX = 0; screenX < screenTiles[0].size(); screenX++) {
            if (NULL == screenTiles[screenY][screenX]) {
                continue;
            }

            renderTile(
                    sdlw,
                    screenTiles[screenY][screenX],
                    screenX * g_constants::TILE_WIDTH,
                    screenY * g_constants::TILE_HEIGHT);
        }
    }
}

void renderScreenTilesLayers(
        Sdlw& sdlw,
        const screen_tiles_layers& screenTilesLayers)
{
    for (const screen_tiles& screenTiles : screenTilesLayers) {
        renderScreenTiles(sdlw, screenTiles);
    }
}

void render_player(Sdlw& sdlw, tile_pool& tile_pool_player)
{
    // 0 is used for empty .tmj layer data items. Render the first tile,
    // whose id is 1.
    renderTile(
        sdlw,
        &(tile_pool_player[1]),
        0,
        0);
}

#if DEBUG_VERBOSE
void printScreenTilesLayers(screen_tiles_layers& screenTilesLayers)
{
    Log::d("Printing screenTilesLayers:");
    for (auto& screenTiles : screenTilesLayers) {
        Log::d("Printing screenTiles:");
        for (auto& tilesRow : screenTiles) {
            for (Tile* tilePtr : tilesRow) {
                if (NULL == tilePtr) {
                    std::cout << 0;
                    std::cout << ", ";
                } else {
                    std::cout << tilePtr->getTiledGid();
                    std::cout << ", ";
                }
            }

            std::cout << "\n";
        }
    }
}
#endif

#if DEBUG_VERBOSE
void printTilesFromTilePool(tile_pool& tilePool)
{
    Log::d("Printing 10 first tiles:");
    for (int i = 0; i < 10; ++i) {
        std::string msg = "\n" + tilePool[i].getDump();
        Log::d(msg);
    }

    Log::d("Printing specific tiles:");
    std::string tempMsg = "\n" + tilePool[580].getDump();
    Log::d(tempMsg);
    tempMsg = "\n" + tilePool[700].getDump();
    Log::d(tempMsg);
}
#endif

#if DEBUG
void printConstructedSpritesheets(const spritesheet_pool& spritesheetPool)
{
    for (const Spritesheet& spritesheet : spritesheetPool) {
        std::string msg = "Constructed spritesheet: ";
        msg += spritesheet.get_name();
        msg += " (";
        msg += std::to_string(spritesheet.get_tiled_firstgid());
        msg += ")";
        Log::d(msg);
    }
}
#endif

void game(void)
{
    int ret = -1;

    int cameraY = g_constants::TILES_VERTICAL / 2;
    int cameraX = g_constants::TILES_HORIZONTAL / 2;

    bool quitEventReceived = false;

    Sdlw& sdlw = Sdlw::getReference();

    SDL_Event event;

    tile_pool tile_pool_map;
    tile_pool tile_pool_player;
    tile_id_map tileIdMap;
    texture_pool texturePool;
    spritesheet_pool spritesheetPool;
    Spritesheet spritesheet_player;

    screen_tiles_layers screenTilesLayers;

    (void) tileIdMap;
    (void) texturePool;

    spritesheet_player = {};

    // TODO check file existence

    Log::clear();

    Log::i("Loading map2");
    Map currentMap = Map("maps/map2_16x16_redone.tmj");

    Log::i("Initializing rendering");
    try {
        Sdlw::initRendering();
    } catch(std::exception& e) {
        std::cerr << "Exception while initializing rendering: " << e.what();
        throw e;
    }

    Log::i("Loading spritesheets");
    GraphicsUtil::load_spritesheets_map(spritesheetPool, currentMap);
    GraphicsUtil::load_spritesheet_player(spritesheet_player);

#if DEBUG
    printConstructedSpritesheets(spritesheetPool);
#endif // DEBUG

    Log::i("Generating tiles");
    try {
        GraphicsUtil::generate_tiles_map(spritesheetPool, tile_pool_map);
        GraphicsUtil::generate_tiles_player(spritesheet_player, tile_pool_player);
    } catch (std::exception const& e) {
        std::string msg = "Exception while generating tiles from spritesheets: ";
        msg += e.what();
        msg += "\n";
        Log::e(msg);
        throw e;
    }

#if DEBUG_VERBOSE
    printTilesFromTilePool(tile_pool_map);
#endif

    Log::i("Entering main loop");
    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

        sdlw.renderClear();

        fillScreenTiles(tile_pool_map,
			currentMap,
			cameraX,
			cameraY,
			screenTilesLayers);

#if DEBUG_VERBOSE
        printScreenTilesLayers(screenTilesLayers);
#endif

        renderScreenTilesLayers(sdlw, screenTilesLayers);
        render_player(sdlw, tile_pool_player);

        sdlw.renderPresent();

        if (g_constants::DEBUG_FRAME_STEPPING) {
            std::string readHere = "";
            std::getline(std::cin, readHere);
        }
    }

#if 0
    mainLoop();
#endif

    sdlw.destroy();
}

// Disable name mangling so that SDL can find and redefine main.
// https://djrollins.com/2016/10/02/sdl-on-windows/
extern "C" int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    try {
        game();
    } catch(std::exception const& e) {
        std::string msg = "Terminating due to unhandled exception: ";
        msg += e.what();
        Log::e(msg);
    }

    return 0;
}
