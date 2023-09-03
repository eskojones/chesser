#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SDL2/SDL_timer.h"
#include "font_chess.h"
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
    Font *font_small, *font_medium, *font_chess;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Surface *original;
    SDL_Surface *tempSurface, *screen;
} State;

State *state;


void sdl_init (State *state, char *title, int width, int height, float scale) {
    state->font_small = (Font *)malloc(sizeof(Font));
    state->font_medium = (Font *)malloc(sizeof(Font));
    state->font_chess = (Font *)malloc(sizeof(Font));
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
    state->font_chess->data = font_chess;
    state->font_chess->width = 10;
    state->font_chess->height = 10;
    state->font_chess->spacing = 1;
    state->font_chess->ascii_start = 65;
    state->font_chess->ascii_end = 91;

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


void draw_chess_board (Surface *surface, int x, int y, int w, int h, uint16_t white, uint16_t black) {
    Rect dstRect = {
        x,
        y,
        w / 8,
        h / 8
    };
    Rect srcRect = {
        0,
        0,
        4,
        4
    };
    Surface *whiteFill = surface_create(4, 4);
    Surface *blackFill = surface_create(4, 4);
    surface_fill(whiteFill, white);
    surface_fill(blackFill, black);
    Surface *current = whiteFill;
    for (int y = 0 ; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            surface_scaleblit(surface, current, &dstRect, &srcRect);
            dstRect.x += dstRect.w;
            if (current == whiteFill) current = blackFill;
            else if (current == blackFill) current = whiteFill;
        }
        dstRect.y += dstRect.h;
        dstRect.x = 0;
            if (current == whiteFill) current = blackFill;
            else if (current == blackFill) current = whiteFill;
    }
    surface_destroy(whiteFill);
    surface_destroy(blackFill);
}


int main (int argc, char **argv) {
    srand(1337);
    state = (State *)malloc(sizeof(State));
    sdl_init(state, "Chesser", 12 * 8 + 80, 12 * 8 + 10, 6);

    ChessGame *game = chess_newgame();

    int last_sx, last_sy, last_dx, last_dy;


    int running = 1, lastFrame = 0, frame = 0, fps, idx = 0, game_num = 1;
    char fpsStr[10], status_line[30] = "";
    struct timeval tv;
    time_t lastTime = gettimeofday(&tv, NULL);
    while (running) {
        SDL_Event ev;
        while(SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:              running = 0; break;
                case SDL_KEYDOWN:           running = 0; break;
                //case SDL_MOUSEBUTTONDOWN:   running = 0; break;
                case SDL_WINDOWEVENT_CLOSE: running = 0; break;
            }
        }

        surface_fill(state->original, 0x0000);
        draw_chess_board(state->original, 0, 0, 12*8, 12*8, GRAY, BROWN);


        for (int i = 0; i < 64; i++) {
            int tx = i % 8;
            int ty = floor((float)i / 8);
            if (game->board[i] != NULL) {
                char pieceStr[] = { chess_piece_chars[game->board[i]->type], 0 };
                font_print(state->original, state->font_chess, pieceStr, tx * 12 + 1,  ty * 12 + 1, game->board[i]->colour == WHITE ? 0xffff : 0x0000);
            }
        }

        gettimeofday(&tv, NULL);
        if (tv.tv_sec != lastTime) {
            fps = lastFrame;
            lastFrame = 0;
            lastTime = tv.tv_sec;
            sprintf(fpsStr, "%d FPS", fps);
        }

        bool didMove = false;
        while (!didMove) {
            int c = 0, p_idx;
            ChessPiece *p = NULL;
            while (p == NULL && c < 32) {
                p_idx = ((idx++) * 2531) % 32;
                p = &game->pieces[p_idx];
                if (p->type == INVALID) p = NULL;
                c++;
            }

            if (c == 32) {
                printf("No piece available\n");
                return 0;
            }

            int board_idx = rand() % 64;//((p->counter++ + game->moves + lastFrame) * 2531) % 64;
            int sx = p->column, 
                sy = p->row, 
                dx = floor((float)board_idx / 8.0f),
                dy = board_idx % 8;
            ChessTurnResult *result = chess_turn(game, sx, sy, dx, dy);
            if (result != NULL && result->code >= CH_MOVE_SUCCESS) {
                if (result->code == CH_MOVE_TAKE) {
                    snprintf(status_line, 25, "%d: %c -> %c (%c%c)", 
                        game->moves, 
                        chess_piece_chars[result->piece_type],
                        chess_piece_chars[result->target_type],
                        chess_board_columns[dx],
                        chess_board_rows[dy]
                    );
                    if (result->target_type == KING) {
                        //game over
                        chess_free(game);
                        game = chess_newgame();
                        SDL_Delay(1000);
                    }
                }
                last_sx = sx;
                last_sy = sy;
                last_dx = dx;
                last_dy = dy;

                didMove = true;
                
                SDL_Delay(10);
            // } else {
            //     surface_line(
            //         state->original,
            //         sx * 12 + 6, sy * 12 + 6, 
            //         dx * 12 + 6, dy * 12 + 6,  
            //         RED
            //     );
            }
            if (didMove) free(result);
        }


        
        if (game->moves) {
            surface_line(
                state->original,
                last_sx * 12 + 6, last_sy * 12 + 6, 
                last_dx * 12 + 6, last_dy * 12 + 6,  
                GREEN
            );
        }

        //font_print(state->original, state->font_small, fpsStr, 1, 1, GREEN);
        font_print(state->original, state->font_medium, status_line, 1, state->original->height - 8, GREEN);
        convert_surface(state->tempSurface, state->original);
        SDL_BlitScaled(state->tempSurface, NULL, state->screen, NULL);
        SDL_UpdateWindowSurface(state->window);
        SDL_Delay(100);
        lastFrame++;
    }



    chess_free(game);
    return 0;
}
