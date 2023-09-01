#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>

#include "font.h"
#include "surface.h"
#include "chess.h"

//#define WHITE   0xFFFF
//#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0xF81F
#define GRED    0xFFE0
#define GBLUE   0x07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0xBC40
#define BRRED   0xFC07
#define GRAY    0x8430


void SDL_SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
        + y * surface->pitch
        + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}


void convert_surface (SDL_Surface *dst, Surface *src) {
    SDL_LockSurface(dst);
    for (int y = 0, i = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            uint16_t p = surface_getpixel(src, x, y);
            p = ((p << 8) & 0xff00) | (p >> 8);
            uint8_t r = (p >> 11) * 8;
            uint8_t g = ((p & 0x07c0) >> 6) * 8;
            uint8_t b = (p & 0x001f) * 8;
            SDL_SetPixel(dst, x, y, SDL_MapRGB(dst->format, r, g, b));
        }
    }
    SDL_UnlockSurface(dst);
}

typedef struct {
    Font *font_small, *font_medium;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Surface *original;
    SDL_Surface *tempSurface, *screen;
} State;

State *state;


void sdl_init (State *state, char *title, int width, int height, float scale) {
    state->font_small = (Font *)malloc(sizeof(Font));
    state->font_medium = (Font *)malloc(sizeof(Font));
    //setup fonts for font_print()
    state->font_small->data = font_5x5;
    state->font_small->width = 5;
    state->font_small->height = 5;
    state->font_small->spacing = 1;
    state->font_small->ascii_start = 32;
    state->font_small->ascii_end = 126;
    state->font_medium->data = font_7x7;
    state->font_medium->width = 7;
    state->font_medium->height = 7;
    state->font_medium->spacing = 1;
    state->font_medium->ascii_start = 32;
    state->font_medium->ascii_end = 126;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return;
    }

    state->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        (int)(width * scale), (int)(height * scale),
        SDL_WINDOW_SHOWN
    );
    if (state->window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetSwapInterval(0);

    //our "pico" screen
    state->original = surface_create(width, height);
    state->tempSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    //the sdl screen surface
    state->screen = SDL_GetWindowSurface(state->window);
    return;
}


int main (int argc, char **argv) {
    srand(1337);
    state = (State *)malloc(sizeof(State));
    sdl_init(state, "test", 8 * 8, 8 * 8, 16);



    ChessGame *game = chess_newgame();


    int running = 1, lastFrame = 0, frame = 0, fps;
    char fpsStr[10];
    struct timeval tv;
    time_t lastTime = gettimeofday(&tv, NULL);
    while (running) {
        SDL_Event ev;
        while(SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:              running = 0; break;
                //case SDL_KEYDOWN:           running = 0; break;
                //case SDL_MOUSEBUTTONDOWN:   running = 0; break;
                case SDL_WINDOWEVENT_CLOSE: running = 0; break;
            }
        }
        surface_fill(state->original, 0);




        for (int i = 0; i < 64; i++) {
            int tx = i % 8;
            int ty = floor((float)i / 8);
            if (game->board[i] == NULL) {
                //printf("#");
                //font_print(state->original, state->font_medium, "#", tx * 8, ty * 8, tx % 2 == 0 ? (ty % 2 == 0 ? 0xffff : 0x0000) : (ty % 2 == 0 ? 0x0000 : 0xffff));
            } else {
                //printf("%c", (game->board[i]->colour == WHITE ? game->board[i]->type - 32 : game->board[i]->type));
                char pieceStr[] = { game->board[i]->type, 0 };
                font_print(state->original, state->font_medium, pieceStr, tx * 8, ty * 8, game->board[i]->colour == WHITE ? 0xffff : GRAY);
            }
            //if (i % 8 == 7) printf("\n");
        }


        gettimeofday(&tv, NULL);
        if (tv.tv_sec != lastTime) {
            fps = lastFrame;
            lastFrame = 0;
            lastTime = tv.tv_sec;
            sprintf(fpsStr, "%d FPS", fps);
        }

        //int idx;
        ChessPiece *p = &game->pieces[rand() % 32];
        // while (p == NULL) {
        //     idx = rand() % 32;
        //     if (game->pieces[idx].type != INVALID && game->pieces[idx].colour == game->moves % 2) {
        //         p = &game->pieces[idx];
        //         break;
        //     }
            
        // }
        // printf("%c %d\n", p->type, idx);
        int sx = p->column, sy = p->row, dx = p->column + (-2 + rand() % 4), dy = p->row + (-2 + rand() % 4);
        if (chess_turn(game, sx, sy, dx, dy)) {
            printf("%d\n", game->moves);
        }
        
        //font_print(state->original, state->font_small, fpsStr, 1, 1, GREEN);
        convert_surface(state->tempSurface, state->original);
        SDL_BlitScaled(state->tempSurface, NULL, state->screen, NULL);
        SDL_UpdateWindowSurface(state->window);
        SDL_Delay(1);
        lastFrame++;
    }



    chess_free(game);
    return 0;
}
