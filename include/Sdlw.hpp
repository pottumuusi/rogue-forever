/*
 * Sdlw is a wrapper around SDL library.
 */

#ifndef SDLW_HPP_DEFINED
#define SDLW_HPP_DEFINED

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

#ifdef _WIN32
#include <SDL2/SDL.h>
#endif

#include <cstdint>
#include <memory>
#include <string>

struct InterfaceSdlw {
    void (*moduleCleanupSdlw) (void);
    void (*init) (std::uint32_t flags);
    void (*set_hint) (std::string name, std::string value);
    void (*img_init) (int flags);
    void (*create_main_window) (
            std::string title,
            int x,
            int y,
            int w,
            int h,
            std::uint32_t flags);
    void (*create_main_renderer) (int index, std::uint32_t flags);
    void (*set_render_draw_color) (
            std::uint8_t r,
            std::uint8_t g,
            std::uint8_t b,
            std::uint8_t a);
    void (*destroy) (void); // TODO figure out if destroy() is redundant with moduleCleanupSdlw()
    void (*render_clear) (void);
    void (*render_present) (void);
    int (*render_copy) (
            std::shared_ptr<SDL_Texture> texture,
            const SDL_Rect* srcrect,
            const SDL_Rect* dstrect);
    std::shared_ptr<SDL_Texture> (*img_load_texture_shared) (std::string file);
    void (*destroy_texture) (std::shared_ptr<SDL_Texture> texture);
    void (*init_rendering) (void);
};

void constructInterfaceSdlw(void);

#endif // SDLW_HPP_DEFINED
