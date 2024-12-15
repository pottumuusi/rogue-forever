#ifndef SPRITESHEET_HPP_DEFINED
#define SPRITESHEET_HPP_DEFINED

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#include <SDL2/SDL.h>
#endif

#include <memory>
#include <string>
#include <vector>

#include "Json.hpp"
#include "Map.hpp"

const int SPRITESHEET_POOL_SIZE = 8;

class Spritesheet;

using spritesheet_pool = std::vector<Spritesheet>;

class Spritesheet {
public:
    Spritesheet() = default;
    ~Spritesheet() = default;
    Spritesheet(std::string _name);
    Spritesheet(std::string _name, Map& map);

    std::string get_name(void) const;
    std::shared_ptr<SDL_Texture> get_texture(void);
    cJSON* get_json(void);
    nlohmann::json get_json_nlohmann(void);
    int get_tiled_firstgid(void) const;

    static const std::array<std::string, SPRITESHEET_POOL_SIZE> spritesheet_names;
    static const std::string spritesheet_name_player;

private:
    std::string name;
    std::shared_ptr<SDL_Texture> texture;
    cJSON* json; // TODO rename 'json_spritesheet'
    nlohmann::json json_nlohmann;
    int tiledFirstgid;

    void load_texture(std::string pathImage);
    void load_json(std::string pathJson);
    void load_json_nlohmann(std::string pathJson);
    void fetch_firstgid(Map& map);
    void fetch_firstgid_nlohmann(Map& map);
    void set_tiled_firstgid(int new_tiled_firstgid);
};

#endif
