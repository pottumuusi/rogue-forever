#include <boost/json/src.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const char PROGRAM_NAME[] = "Rogue Forever";

#if 0
void mainLoop(void) {
}
#endif

int main(void) {
    const std::string assetsPrefix = "assets/";
    const std::string imagePathWallStoneGray1 =
        assetsPrefix + "dc-dngn/wall/stone_gray1.png";

    const int imgFlags = IMG_INIT_PNG;

    const int renderingDriver = -1; // -1 initializes the first driver
                                    // supporting requested flags

    SDL_Rect* srcRect = NULL;
    SDL_Rect* dstRect = NULL;

    int ret = -1;
    bool quitEventReceived = false;

    SDL_Window* mainWindow = NULL;
    SDL_Texture* texture = NULL;
    SDL_Surface* mainWindowSurface = NULL;
    SDL_Surface* temporarySurface = NULL;
    SDL_Surface* imageSurface = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        goto error_exit;
    }

    ret = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    if (ret < 0) {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    ret = IMG_Init(imgFlags);
    if (0 == ret) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        goto error_exit;
    }

    mainWindow = SDL_CreateWindow(
            PROGRAM_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
    if (NULL == mainWindow) {
        printf("Failed to create window: %s\n", SDL_GetError());
        goto error_exit;
    }

    renderer = SDL_CreateRenderer(
            mainWindow,
            renderingDriver,
            SDL_RENDERER_ACCELERATED );
    if(NULL == renderer) {
        printf("Failed to create renderer. SDL Error: %s\n", SDL_GetError());
        goto error_exit;
    }

    mainWindowSurface = SDL_GetWindowSurface(mainWindow);
    if (NULL == mainWindowSurface) {
        printf("Failed to get window surface: %s\n", SDL_GetError());
        goto error_exit;
    }

    ret = SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (ret < 0) {
        printf("Failed to set render draw color: %s\n", SDL_GetError());
        goto error_exit;
    }

    texture = IMG_LoadTexture(renderer, imagePathWallStoneGray1.c_str());
    if (NULL == texture) {
        printf("Failed to load texture from file %s\n",
                imagePathWallStoneGray1.c_str());
        goto error_exit;
    }

#if 0
    printf("Loading asset: %s\n", image_path_wall_stone_gray1.c_str());
    temporarySurface = IMG_Load(image_path_wall_stone_gray1.c_str());
    if (NULL == temporarySurface) {
        printf("Unable to load asset: %s Error: %s",
                image_path_wall_stone_gray1.c_str(),
                IMG_GetError());
        goto error_exit;
    }

    imageSurface = SDL_ConvertSurface(
            temporarySurface,
            mainWindowSurface->format,
            0);
    if (NULL == imageSurface) {
        printf("Failed to convert surface to format of main "
                "windows surface: %s\n", SDL_GetError());
        goto error_exit;
    }

    SDL_FreeSurface(temporarySurface);
    temporarySurface = NULL;
#endif

    while (!quitEventReceived) {
        do {
            ret = SDL_PollEvent(&event);

            if (SDL_QUIT == event.type) {
                quitEventReceived = true;
            }
        } while (0 != ret);

#if 0
        SDL_BlitSurface(imageSurface, NULL, mainWindowSurface, NULL);

        SDL_UpdateWindowSurface(mainWindow);
#endif

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, srcRect, dstRect);

        SDL_RenderPresent(renderer);
    }

#if 0
    mainLoop();
#endif

    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(mainWindow);
    mainWindow = NULL;

    IMG_Quit();
    SDL_Quit();

    return 0;

error_exit:

    if (NULL != imageSurface) {
        SDL_FreeSurface(imageSurface);
        imageSurface = NULL;
    }

    if (NULL != renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (NULL != mainWindow) {
        SDL_DestroyWindow(mainWindow);
        mainWindow = NULL;
    }

    IMG_Quit();
    SDL_Quit();

    return -1;
}
