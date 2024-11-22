#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MENU_WIDTH 500
#define MENU_HEIGHT 227
#define MENU_OPTION_WIDTH 306
#define MENU_OPTION_HEIGHT 32


typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
} MenuOption;

void renderMenu(SDL_Renderer *renderer, MenuOption options[], int numOptions, int selectedOption) {
    for (int i = 0; i < numOptions; i++) {
        SDL_RenderCopy(renderer, options[i].texture, NULL, &options[i].rect);
    }
}

/*SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* tmpSurface = SDL_LoadBMP(filePath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return texture;
}*/

int showMenu() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("No se pudo inicializar SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Menu Simple",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        printf("No se pudo crear la ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("No se pudo crear el renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Texture* menuTxt = loadTexture(renderer, "data/pac-man-logo.bmp");
    SDL_Rect menuRect = { .x=(SCREEN_WIDTH - MENU_WIDTH)/2, .y=50, .w = MENU_WIDTH, .h = MENU_HEIGHT };

    SDL_Texture* pacmanTxt = loadTexture(renderer, "data/pacman1.bmp");
    SDL_Rect pacmanRect = { .x=200, .y=300 + MENU_OPTION_HEIGHT/4, .w = 26, .h = 26 };

    // Definir opciones del menú
    MenuOption options[] = {
        {loadTexture(renderer, "data/play_button.bmp"), {247, 300, MENU_OPTION_WIDTH, MENU_OPTION_HEIGHT}},
        {loadTexture(renderer, "data/load_button.bmp"), {247, 300+60, MENU_OPTION_WIDTH, MENU_OPTION_HEIGHT}},
        {loadTexture(renderer, "data/caster_button.bmp"), {247, 300+120, MENU_OPTION_WIDTH, MENU_OPTION_HEIGHT}},
        {loadTexture(renderer, "data/quit_button.bmp"), {247, 300+180, MENU_OPTION_WIDTH, MENU_OPTION_HEIGHT}}
    };
    const int numOptions = sizeof(options) / sizeof(MenuOption);

    int selectedOption = 0;
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                selectedOption=3;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + numOptions) % numOptions;
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % numOptions;
                        break;
                    case SDLK_RETURN:
                        running = false;
                        break;
                }
            }
        }

        pacmanRect.y = 300 + 60*selectedOption + MENU_OPTION_HEIGHT/4;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, menuTxt, NULL, &menuRect);

        renderMenu(renderer, options, numOptions, selectedOption);

        SDL_RenderCopy(renderer, pacmanTxt, NULL, &pacmanRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(1/60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return selectedOption;
}
