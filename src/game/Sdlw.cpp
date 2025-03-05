#ifdef __linux__
#include <SDL2/SDL_image.h>
#endif

#ifdef _WIN32
#include <SDL2/SDL_image.h>
#endif

#include <iostream>
#include <string>

#include "Constants.hpp"
#include "Log.hpp"
#include "Sdlw.hpp"

static void moduleCleanupSdlw(void);
static void init(std::uint32_t flags);
static void set_hint(std::string name, std::string value);
static void img_init(int flags);
static void create_main_window(
        std::string title,
        int x,
        int y,
        int w,
        int h,
        std::uint32_t flags);
static void create_main_renderer(int index, std::uint32_t flags);
static void set_render_draw_color(
        std::uint8_t r,
        std::uint8_t g,
        std::uint8_t b,
        std::uint8_t a);
static void destroy(void);
static void render_clear(void);
static void render_present(void);
static int render_copy(
        std::shared_ptr<SDL_Texture> texture,
        const SDL_Rect* srcrect,
        const SDL_Rect* dstrect);
static std::shared_ptr<SDL_Texture> img_load_texture_shared(std::string file);
static void destroy_texture(std::shared_ptr<SDL_Texture> texture);
static void init_rendering(void);

struct InterfaceSdlw Sdlw = {0};

static SDL_Window* main_window;
static SDL_Renderer* main_renderer;

void
constructInterfaceSdlw(void)
{
    Sdlw.moduleCleanupSdlw = moduleCleanupSdlw;

    Sdlw.init = init;
    Sdlw.set_hint = set_hint;
    Sdlw.img_init = img_init;
    Sdlw.create_main_window = create_main_window;
    Sdlw.create_main_renderer = create_main_renderer;
    Sdlw.set_render_draw_color = set_render_draw_color;
    Sdlw.destroy = destroy;
    Sdlw.render_clear = render_clear;
    Sdlw.render_present = render_present;
    Sdlw.render_copy = render_copy;
    Sdlw.img_load_texture_shared = img_load_texture_shared;
    Sdlw.destroy_texture = destroy_texture;
    Sdlw.init_rendering = init_rendering;
}

static void
moduleCleanupSdlw(void)
{
    throw std::runtime_error("Sdlw.moduleCleanupSdlw is not yet implemented");
}

#if 0
// TODO: Remove this reference code when in use elsewhere

Sdlw::Sdlw(Sdlw&& other)
    : main_window(std::exchange(other.main_window, nullptr))
    , main_renderer(std::exchange(other.main_renderer, nullptr))
{
}

Sdlw& Sdlw::operator=(Sdlw&& other)
{
    std::swap(main_window, other.main_window);
    std::swap(main_renderer, other.main_renderer);
    return *this;
}
#endif

/*
 * \exception throws std::runtime_error on failure
 */
static void
init_rendering(void)
{
    std::string msg = "";

    const int img_flags = IMG_INIT_PNG;

    // -1 initializes the first driver supporting requested flags
    const int rendering_driver = -1;

    Sdlw.init(SDL_INIT_VIDEO);

    Sdlw.set_hint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    Sdlw.img_init(img_flags);

    Sdlw.create_main_window(
            g_constants::PROGRAM_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            g_constants::SCREEN_WIDTH,
            g_constants::SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    Sdlw.create_main_renderer(
            rendering_driver,
            SDL_RENDERER_ACCELERATED);

    Sdlw.set_render_draw_color(0xFF, 0xFF, 0xFF, 0xFF);
}

static void
init(std::uint32_t flags)
{
    int ret = -1;
    std::string msg = "";

    ret = SDL_Init(flags);
    if (ret < 0) {
        msg = "Failed to initialize SDL: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

static void
destroy(void)
{
    if (NULL != main_renderer) {
        SDL_DestroyRenderer(main_renderer);
        main_renderer = NULL;
    }

    if (NULL != main_window) {
        SDL_DestroyWindow(main_window);
        main_window = NULL;
    }

    IMG_Quit();
    SDL_Quit();
}

static void
set_hint(std::string name, std::string value)
{
    int ret = -1;

    ret = SDL_SetHint(name.c_str(), value.c_str());
    if (ret < 0) {
        std::cout << WARN << "Setting SDL hint " << name << " failed";
    }
}

static void
img_init(int flags)
{
    int ret = -1;
    std::string msg = "";

    ret = IMG_Init(flags);
    if (0 == ret) {
        msg = "Failed to initialize SDL_image: ";
        msg += IMG_GetError();
        throw std::runtime_error(msg);
    }
}

static void
create_main_window(
        std::string title,
        int x,
        int y,
        int w,
        int h,
        std::uint32_t flags)
{
    std::string msg = "";

    main_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    if (NULL == main_window) {
        msg = "Failed to create window: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

static void
create_main_renderer(int index, std::uint32_t flags)
{
    std::string msg = "";

    main_renderer = SDL_CreateRenderer(main_window, index, flags);
    if(NULL == main_renderer) {
        msg = "Failed to create renderer. SDL Error: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

static void
set_render_draw_color(
        std::uint8_t r,
        std::uint8_t g,
        std::uint8_t b,
        std::uint8_t a)
{
    int ret = -1;
    std::string msg = "";

    ret = SDL_SetRenderDrawColor(main_renderer, r, g, b, a);
    if (ret < 0) {
        msg = "Failed to set render draw color: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }
}

static void
render_clear(void)
{
    SDL_RenderClear(main_renderer); // TODO handle return value
}

static void
render_present(void)
{
    SDL_RenderPresent(main_renderer); // TODO handle return value
}

static int
render_copy(
        std::shared_ptr<SDL_Texture> texture,
        const SDL_Rect* srcrect,
        const SDL_Rect* dstrect)
{
    return SDL_RenderCopy(main_renderer, texture.get(), srcrect, dstrect);
}

static std::shared_ptr<SDL_Texture>
img_load_texture_shared(std::string file)
{
    std::shared_ptr<SDL_Texture> texture = NULL;
    std::string msg = "";

    texture = std::shared_ptr<SDL_Texture>(
            IMG_LoadTexture(main_renderer, file.c_str()),
            [](SDL_Texture* p) {
                SDL_DestroyTexture(p);
            });
    if (NULL == texture.get()) {
        msg = "Failed to load texture: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
    }

    return texture;
}

static void
destroy_texture(std::shared_ptr<SDL_Texture> texture)
{
    SDL_DestroyTexture(texture.get());
}
