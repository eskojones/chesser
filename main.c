#include <SDL_events.h>
#include <SDL_mouse.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include "font_chess.h"
#include "font.h"
#include "surface.h"
#include "chess.h"
#include "player.h"
#include "list.h"



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


typedef struct {
    Font *font_small, *font_medium, *font_chess;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Surface *original;
    SDL_Surface *tempSurface, *screen;
    int board_x, board_y;
    int frame;
    ChessGame *game;
    bool running;
    char status_line[32];
    int sx,sy,dx,dy,index;
    bool keydown;
    SDL_Scancode keycode;
    bool mousebuttondown;
    bool mousebuttonleft;
    bool mousebuttonmiddle;
    bool mousebuttonright;
    int mouse_x, mouse_y;
} State;



void SDL_SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
        + y * surface->pitch
        + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}


void convert_surface (SDL_Surface *dst, Surface *src) {
    SDL_LockSurface(dst);
    for (int y = 0; y < src->height; y++) {
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


void init (State *state, char *title, int width, int height, float scale) {
    srand(1337);
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
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        (int)((float)width * scale), (int)((float)height * scale),
        SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS|SDL_WINDOW_MOUSE_GRABBED|SDL_WINDOW_MOUSE_CAPTURE
    );
    if (state->window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetSwapInterval(0);

    state->original = surface_create(width, height);
    state->tempSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    //the sdl screen surface
    state->screen = SDL_GetWindowSurface(state->window);

    SDL_ShowCursor(0);
}

void game_reset (State *state) {
    if (state->game != NULL) chess_free(state->game);
    state->game = chess_newgame();
    state->game->players[0] = player_create(state->game, WHITE);
    state->game->players[1] = player_create(state->game, BLACK);
    state->game->players[0]->turn = player_turn_random;
    state->game->players[1]->turn = player_turn_random;
    //LL_MEM();
}

void draw_chess_board (Surface *surface, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t white, uint16_t black) {
    Rect dstRect = { x, y, w / 8, h / 8 };
    Rect srcRect = { 0, 0, 4, 4 };
    Surface *whiteFill = surface_create(4, 4);
    Surface *blackFill = surface_create(4, 4);
    surface_fill(whiteFill, white);
    surface_fill(blackFill, black);
    Surface *current = whiteFill;
    for (int yy = 0 ; yy < 8; yy++) {
        for (int xx = 0; xx < 8; xx++) {
            surface_scaleblit(surface, current, &dstRect, &srcRect);
            dstRect.x += dstRect.w;
            current = current == whiteFill ? blackFill : whiteFill;
        }
        dstRect.y += dstRect.h;
        dstRect.x = x;
        current = current == whiteFill ? blackFill : whiteFill;
    }
    surface_destroy(whiteFill);
    surface_destroy(blackFill);
}


void input (State *state, uint64_t delta) {
    SDL_Event ev;
    while(SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_QUIT:              state->running = false; break;
            case SDL_KEYDOWN:           
                if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    state->running = false;
                }
                state->keydown = true; 
                state->keycode = ev.key.keysym.scancode;
                break;
            case SDL_KEYUP:
                state->keydown = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                state->mousebuttondown = true;
                state->mouse_x = ev.button.x;
                state->mouse_y = ev.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                state->mousebuttondown = false;
                break;
            case SDL_WINDOWEVENT_CLOSE: state->running = false; break;
        }
    }

    uint32_t buttons = SDL_GetMouseState(&state->mouse_x, &state->mouse_y);
    state->mousebuttonleft = buttons & SDL_BUTTON(1);
    state->mousebuttonmiddle = buttons & SDL_BUTTON(2);
    state->mousebuttonright = buttons & SDL_BUTTON(3);
}


void update (State *state, uint64_t delta) {
    bool didMove = false;
    while (!didMove) {
        chess_do_turn(state->game);
        if (state->game->moves == 0) return;
        ChessTurnResult *result = (ChessTurnResult *) ll_tail(state->game->log)->data;
        if (result != NULL && result->code >= CH_MOVE_SUCCESS) {
            if (result->code == CH_MOVE_TAKE) {
                snprintf(state->status_line, 25, "%d: %c -> %c (%c%c)",
                         state->game->moves,
                         chess_piece_chars[result->piece_type],
                         chess_piece_chars[result->target_type],
                         chess_board_columns[result->target_column],
                         chess_board_rows[result->target_row]
                );
                if (result->target_type == KING) {
                    //game over
                    game_reset(state);
                }
            } else if (result->code == CH_MOVE_PROMOTE) {
                //pawn promotion
                result->piece->type = QUEEN;
            }
            state->sx = result->piece_column;
            state->sy = result->piece_row;
            state->dx = result->target_column;
            state->dy = result->target_row;
            didMove = true;
        }
    }



    return ;
/*
    bool didMove = false;
    while (!didMove) {
        int c = 0, p_idx;
        ChessPiece *p = NULL;
        while (p == NULL && c < 32) {
            p_idx = ((state->index++) * 2531) % 32;
            p = &state->game->pieces[p_idx];
            if (p->status == TAKEN) p = NULL;
            c++;
        }

        if (c >= 32) {
            printf("No piece available\n");
            return;
        }

        int board_idx = rand() % 64;
        int sx = p->column, 
            sy = p->row, 
            dx = (int)floorf((float)board_idx / 8.0f),
            dy = board_idx % 8;
        ChessTurnResult *result = chess_turn(state->game, sx, sy, dx, dy);
        if (result != NULL && result->code >= CH_MOVE_SUCCESS) {
            if (result->code == CH_MOVE_TAKE) {
                snprintf(state->status_line, 25, "%d: %c -> %c (%c%c)", 
                    state->game->moves, 
                    chess_piece_chars[result->piece_type],
                    chess_piece_chars[result->target_type],
                    chess_board_columns[dx],
                    chess_board_rows[dy]
                );
                if (result->target_type == KING) {
                    //game over
                    chess_free(state->game);
                    state->game = chess_newgame();
                }
            } else if (result->code == CH_MOVE_PROMOTE) {
                //pawn promotion
                p->type = QUEEN;
            }
            state->sx = sx;
            state->sy = sy;
            state->dx = dx;
            state->dy = dy;

            didMove = true;
            
        }
        if (didMove) free(result);
    }*/
}


void render (State *state, uint64_t delta) {
    surface_fill(state->original, 0x0000);
    draw_chess_board(
        state->original, 
        state->board_x, 
        state->board_y, 
        12*8, 12*8, 
        GRAY, BROWN
    );

    for (int i = 0; i < 64; i++) {
        int tx = i % 8;
        int ty = (int)floorf((float)i / 8);
        if (state->game->board[i] != NULL) {
            char pieceStr[] = { chess_piece_chars[state->game->board[i]->type], 0 };
            font_print(
                state->original, 
                state->font_chess, 
                pieceStr, 
                tx * 12 + 1 + state->board_x,  
                ty * 12 + 1 + state->board_y, 
                state->game->board[i]->colour == WHITE ? 0xffff : 0x0000
            );
        }
    }

    if (state->game->moves) {
        surface_line(
            state->original,
            state->sx * 12 + 6 + state->board_x, state->sy * 12 + 6 + state->board_y, 
            state->dx * 12 + 6 + state->board_x, state->dy * 12 + 6 + state->board_y,  
            GREEN
        );
    }

    //font_print(state->original, state->font_small, fpsStr, 1, 1, GREEN);
    font_print(
        state->original, 
        state->font_medium, 
        state->status_line, 
        1, state->original->height - 8, 
        GREEN
    );

    int mx = (int)floorf((float)state->mouse_x / 6.0f);
    int my = (int)floorf((float)state->mouse_y / 6.0f);
    surface_circle(state->original, mx, my, 1, GREEN);
    // surface_line(
    //     state->original,
    //     (int)floor((float)state->mouse_x / 6.0f), (int)floor((float)state->mouse_y / 6.0f), 
    //     (int)floor((float)state->mouse_x / 6.0f)+1, (int)floor((float)state->mouse_y / 6.0f)+1,  
    //     GREEN
    // );

    convert_surface(state->tempSurface, state->original);
    SDL_BlitScaled(state->tempSurface, NULL, state->screen, NULL);
    SDL_UpdateWindowSurface(state->window);
    state->frame++;
}


int main (int argc, char **argv) {
    State *state = (State *)malloc(sizeof(State));
    init(state, "Chesser", 12 * 8 * 2, 12 * 8 + 8, 6);
    state->board_x = 4 * 12;//(int)((float)state->screen->w / 4.0f);
    state->board_y = 0;
    game_reset(state);

    struct timeval tv;
    time_t lastTime = gettimeofday(&tv, NULL);
    uint64_t lastUpdate = tv.tv_usec;
    uint64_t lastRender = tv.tv_usec;

    state->running = true;
    while (state->running) {
        gettimeofday(&tv, NULL);
        input(state, 0);

        if (tv.tv_usec - lastUpdate >= 1 * 1000) {
            //if ((state->mousebuttondown && state->mousebuttonleft) || state->keydown) {
                update(state, tv.tv_usec - lastUpdate);
                state->keydown = false;
                state->mousebuttondown = false;
            //}
            render(state, tv.tv_usec - lastUpdate);
            lastUpdate = tv.tv_usec;
        }

        //SDL_Delay(1);
    }

    chess_free(state->game);
    return 0;
}
