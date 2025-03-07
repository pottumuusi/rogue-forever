#ifndef TILE_HPP_DEFINED
#define TILE_HPP_DEFINED

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#include <SDL2/SDL.h>
#endif

#include <memory>
#include <string>
#include <unordered_map>

#include "Constants.hpp"
#include "Spritesheet.hpp"

struct Tile {
    struct Tile* this_instance;

    // How private variables can be hidden? Could constructor set
    // static variables?
};

void constructTile(void);

class Tile;

using tile_pool = std::unordered_map<int, Tile>;
using tile_id_map = std::array<std::string, g_constants::TILE_AMOUNT>;

#if 0
class Tile {
public:
    Tile() = default;
    ~Tile();
    Tile(
            std::string _name,
            std::shared_ptr<SDL_Texture> _texture,
            const int _sheetX,
            const int _sheetY,
            const int _sheetW,
            const int _sheetH,
            const int _tiledId);
    Tile(
            std::string _name,
            std::shared_ptr<SDL_Texture> _texture,
            const int _sheetX,
            const int _sheetY,
            const int _sheetW,
            const int _sheetH,
            const int _tiledGid,
            const int _tiledId);

    int getScreenX() const;
    int getScreenY() const;
    std::string getName() const;
    std::shared_ptr<SDL_Texture> getSheetTexture();
    int getSheetX() const;
    int getSheetY() const;
    int getSheetW() const;
    int getSheetH() const;
    int getTiledGid() const;
    int getTiledId() const;
    std::string getDump();

private:
    std::string name;
    std::shared_ptr<SDL_Texture> sheetTexture;
    int sheetX;
    int sheetY;
    int sheetW;
    int sheetH;
    int tiledGid;
    int tiledId;
};
#endif

#endif
