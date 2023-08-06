#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#ifdef _MSC_VER
    #define SDL_MAIN_HANDLED
#endif
#include <SDL2/SDL.h>

#include "vector.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;

static SDL_Window *window;
static SDL_Renderer *renderer;
static i32 windowWidth;
static i32 windowHeight;
static i32 color;
bool running = true;

// Graphics
// ---------------------
bool gfxOpenWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        // std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = display_mode.w;
    windowHeight = display_mode.h;
    window = SDL_CreateWindow(NULL, 0, 0, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
    if (!window) {
        // std::cerr << "Error creating SDL window" << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        // std::cerr << "Error creating SDL renderer" << std::endl;
        return false;
    }
    return true;
}

void gfxClearScreen(u32 color) {
    SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);
    SDL_RenderClear(renderer);
}

void gfxCloseWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void gfxRenderFrame() {
    SDL_RenderPresent(renderer);
}

void gfxDrawLine(int x0, int y0, int x1, int y1, u32 color) {
    lineColor(renderer, x0, y0, x1, y1, color);
}

void gfxDrawCircle(int x, int y, int radius, float angle, u32 color) {
    circleColor(renderer, x, y, radius, color);
    lineColor(renderer, x, y, x + cos(angle) * radius, y + sin(angle) * radius, color);
}

void gfxDrawFillCircle(int x, int y, int radius, u32 color) {
    filledCircleColor(renderer, x, y, radius, color);
}

void gfxDrawRect(int x, int y, int width, int height, u32 color) {
    lineColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y - height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y + height / 2.0, x - width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x - width / 2.0, y + height / 2.0, x - width / 2.0, y - height / 2.0, color);
}

void gfxDrawFillRect(int x, int y, int width, int height, u32 color) {
    boxColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
}

/*
void DrawPolygon(int x, int y, const vec2 *vertices, u32 color) {
    for (int i = 0; i < verticesarray.length; i++) {
        int currIndex = i;
        int nextIndex = (i + 1) % verticesarray.length;
        lineColor(renderer, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
    }
    filledCircleColor(renderer, x, y, 1, color);
}

void DrawFillPolygon(int x, int y, const vec2 *vertices, u32 color) {
    Array vx;
    Array vy;
    for (int i = 0; i < vertices.size(); i++) {
        vx.push_back(static_cast<int>(vertices[i].x));
    }
    for (int i = 0; i < vertices.size(); i++) {
        vy.push_back(static_cast<int>(vertices[i].y));
    }
    filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), color);
    filledCircleColor(renderer, x, y, 1, 0xFF000000);
}
*/

void gfxDrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture* texture) {
    SDL_Rect dstRect = {x - (width / 2), y - (height / 2), width, height};
    float rotationDeg = rotation * 57.2958;
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, rotationDeg, NULL, SDL_FLIP_NONE);
}


// System
// ---------------------
void input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
        }
    }
}

void update() {

}

void render() {
    gfxClearScreen(color);
    gfxDrawFillCircle(512, 512, 10, 0xFFFFFFFF);
    gfxRenderFrame();
}

// Entry Point
// ---------------------
int main(int argc, char *argv[]) {
    windowWidth = 800;
    windowHeight = 640;
    color = 0xFF111111;
    gfxOpenWindow();
    vec2 pos = {512, 512};

    while(running != false) {
        input();
        update();
        render();
    }

    gfxCloseWindow();
    return 0;
}

