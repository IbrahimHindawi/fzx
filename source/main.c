#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL.h>

#include "core.h"
#include "vector.h"
#include "fzx/fzx.h"

#include "hkArray.h"

#define FPS 60
const int frameTime = 1000 / FPS;
const float frameTimef32 = 1000.f / FPS;
int framePrevTime;
int frameDelay;

static SDL_Window *window;
static SDL_Renderer *renderer;
static i32 windowWidth;
static i32 windowHeight;

static bool running = true;
static fzxParticle *particle;

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
    lineColor(renderer, x, y, (i16)(x + cos(angle) * radius), (i16)(y + sin(angle) * radius), color);
}

void gfxDrawFillCircle(int x, int y, int radius, u32 color) {
    filledCircleColor(renderer, x, y, radius, color);
}

void gfxDrawRect(int x, int y, int width, int height, u32 color) {
    lineColor(renderer, (i16)(x - width / 2.f), (i16)(y - height / 2.f), (i16)(x + width / 2.f), (i16)(y - height / 2.f), color);
    lineColor(renderer, (i16)(x + width / 2.f), (i16)(y - height / 2.f), (i16)(x + width / 2.f), (i16)(y + height / 2.f), color);
    lineColor(renderer, (i16)(x + width / 2.f), (i16)(y + height / 2.f), (i16)(x - width / 2.f), (i16)(y + height / 2.f), color);
    lineColor(renderer, (i16)(x - width / 2.f), (i16)(y + height / 2.f), (i16)(x - width / 2.f), (i16)(y - height / 2.f), color);
}

void gfxDrawFillRect(int x, int y, int width, int height, u32 color) {
    boxColor(renderer, 
            (i16)(x - width / 2.f), 
            (i16)(y - height / 2.f), 
            (i16)(x + width / 2.f), 
            (i16)(y + height / 2.f), 
            color);
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
    float rotationDeg = rotation * 57.2958f;
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, rotationDeg, NULL, SDL_FLIP_NONE);
}


// System
// ---------------------
void setup() {
    particle = malloc(sizeof(fzxParticle));
    *particle = fzxParticleCreate(512.f, 512.f, 32.f);
    particle->velocity.x = 3.f;
    particle->velocity.y = 3.f;
    // particle->acceleration.x = 1.f;
    particle->acceleration.y = 1.f;
}

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
    // delta time
    frameDelay = frameTime - (SDL_GetTicks() - framePrevTime);
    if(frameDelay > 0) {
        SDL_Delay(frameDelay);
    }
    float deltaTime = (SDL_GetTicks() - framePrevTime) / 1000.f;
    if (deltaTime > frameTime) {
        deltaTime = frameTimef32;
    }
    // printf("ticks: %d, ", SDL_GetTicks());
    framePrevTime = SDL_GetTicks();
    // printf("prev: %d, delay: %d\n", framePrevTime, frameDelay);

    particle->velocity = vec2_scalar_mul_add_scalar(particle->velocity, particle->acceleration, deltaTime);
    particle->position = vec2_add(particle->position, particle->velocity);

    if((particle->position.y <= 0.f) || (particle->position.y >= windowHeight)) {
        particle->velocity.y *= -1.f;
    }
    if((particle->position.x <= 0.f) || (particle->position.x >= windowWidth)) {
        particle->velocity.x *= -1.f;
    }
}

void render() {
    gfxClearScreen(0xFF145288);
    gfxDrawFillCircle(particle->position.x, particle->position.y, 3, 0xFFFFFFFF);
    gfxRenderFrame();
}

void deinit() {
    free(particle);
}

// Entry Point
// ---------------------
int main(int argc, char *argv[]) {
    setup();
    gfxOpenWindow();
    while(running != false) {
        input();
        update();
        render();
    }
    deinit();
    gfxCloseWindow();
    return 0;
}
