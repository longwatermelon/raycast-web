#include "prog.h"
/* #include "audio.h" */
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten/emscripten.h>
/* #include <SDL2/SDL_mixer.h> */

struct Prog *g_prog;

void emloop(void *arg)
{
    prog_mainloop(g_prog);

    if (!g_prog->running && !g_prog->restart)
        emscripten_cancel_main_loop();

    if (!g_prog->running && g_prog->restart)
    {
        SDL_Window *w = g_prog->window;
        SDL_Renderer *r = g_prog->rend;
        prog_cleanup(g_prog);
        g_prog = prog_init(w, r);
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    /* Mix_Init(MIX_INIT_MOD); */

    srand(time(0));

    SDL_Window *window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    g_prog = prog_init(window, rend);

    if (!g_prog)
    {
        fprintf(stderr, "Game failed to initialize, aborting\n");
        goto cleanup;
    }

    emscripten_set_main_loop_arg(emloop, g_prog, -1, 1);

cleanup:
    /* audio_cleanup(); */

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    /* Mix_Quit(); */

    return 0;
}

