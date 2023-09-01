#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "font.h"
#include "surface.h"

#define LCD_WIDTH 160
#define LCD_HEIGHT 128

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024

#define WHITE   0xFFFF
#define BLACK   0x0000
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

#define deg2rad 0.01745329252f
#define rad2deg 0.005555556f

int colours[] = {
    BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};


void surface_rectangle (Surface *surface, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour) {
    surface_line(surface, x, y, x+w, y, colour);
    surface_line(surface, x+w, y, x+w, y+h, colour);
    surface_line(surface, x+w, y+h, x, y+h, colour);
    surface_line(surface, x, y+h, x, y, colour);
}


void SDL_SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
        + y * surface->pitch
        + x * surface->format->BytesPerPixel);
    *target_pixel = pixel;
}


void convert_surface (SDL_Surface *dst, Surface *src) {
    SDL_LockSurface(dst);
    for (int y = 0, i = 0; y < LCD_HEIGHT; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
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



int main(int argc, char* args[]) {
    //setup fonts for font_print()
    Font font_small = {
        font_5x5, //const char* in font_5x5.c
        5, 5, 1, //5x5 font with 1px spacing
        32, 126  //ascii 32 to 126 inclusive
    };

    Font font_medium = {
        font_7x7, 
        7, 7, 1,
        32, 126
    };

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "pico-lcd-base graphics tester",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetSwapInterval(0);

    //our "pico" screen
    Surface *screen = surface_create(LCD_WIDTH, LCD_HEIGHT);
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, LCD_WIDTH, LCD_HEIGHT, 32, 0, 0, 0, 0);
    //the sdl screen surface
    SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

    int running = 1, frame = 0, lastTime = 0, lastFrame = 0, fps = 0;
    float angle = 0.0f;
    char fpsStr[10];
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
        surface_fill(screen, BLACK);

        int offs = frame++ % 16;
        /*
        for (int i = 0; i <= 20; i++) {
            surface_line(
                screen,
                1, offs + (i * 15),
                LCD_WIDTH, offs + (i * 15),
                GREEN
            );
            surface_line(
                screen,
                offs + (i * 15), 1,
                offs + (i * 15), LCD_HEIGHT,
                GREEN
            );
        }
        */
        //SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x0, 0x0, 0x0));


        //for (int i = 0; i < 1000; i++) {
        //    surface_rectangle(screen, rand() % LCD_WIDTH, rand() % LCD_HEIGHT, 20, 10, rand() % 256);
        //}
        for (int i = 0; i < 1; i++) {
            surface_circle(screen, LCD_WIDTH / 2, LCD_HEIGHT / 2, 10, colours[1]);
            surface_rectangle(screen, LCD_WIDTH / 2, LCD_HEIGHT / 2, 200, 200, colours[(1+rand()) % 8]);
        }
        
        struct timeval tv;
        gettimeofday(&tv, NULL);
        if (tv.tv_sec != lastTime) {
            fps = lastFrame;
            lastFrame = 0;
            lastTime = tv.tv_sec;
            sprintf(fpsStr, "%d FPS", fps);
        }
        
        font_print(screen, &font_small, fpsStr, 1, 1, GREEN);
        convert_surface(tempSurface, screen);
        SDL_BlitScaled(tempSurface, NULL, screenSurface, NULL);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(0);
        lastFrame++;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
