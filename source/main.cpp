#include <iostream>
#include <cmath>
#include <vector>
#include <mutex>
#include <thread>
#include <SDL2/SDL.h>
#include "PhysicsWorld.hpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const double BOUNCE = 0.4;
const double AIR_RESISTANCE = 0.1;
const double GRAVITY = 0.01;
bool running = false;
std::thread t1;

void spawnParticles(PhysicsWorld* world){
    int mouse_x, mouse_y;
    float mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
    while(running){
        if(!running) return;
        float mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
        Vec2 pos = Vec2(mouse_x, mouse_y);
        world->particles.push_back(new Particle(pos, pos, Vec2(0, 0), 5));
    }
    return;
}

void cutSticks(PhysicsWorld* world){
    int mouse_x, mouse_y;
    // float mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
    while(running){
        SDL_GetMouseState(&mouse_x, &mouse_y);
        Vec2 pos = Vec2(mouse_x, mouse_y);
        world->cutSticks(pos);
    }
    return;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("PhysEng", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    PhysicsWorld* world = new PhysicsWorld(SCREEN_HEIGHT, SCREEN_WIDTH, BOUNCE, AIR_RESISTANCE, GRAVITY);

    const int NUM_PARTICLES = 60;

    for(int i = 0; i < NUM_PARTICLES; i++){
        for(int j = 0; j < NUM_PARTICLES; j++){
            const int GRID_SIZE = 10;
            Particle* p = new Particle(Vec2(10 + i * GRID_SIZE, 10 + j * GRID_SIZE), Vec2(10 + i * GRID_SIZE, 10 + j * GRID_SIZE), Vec2(0, 0), 5);
            world->particles.push_back(p);
            if(j == 0){
                p->pinned = true;
            }
            
        }
    }

    for(int i = 0; i < NUM_PARTICLES; i++){
        for(int j = 0; j < NUM_PARTICLES; j++){
            if(i < NUM_PARTICLES - 1){
                Stick* s = new Stick(world->particles[i * NUM_PARTICLES + j], world->particles[(i + 1) * NUM_PARTICLES + j], 1, 1.5);
                world->sticks.push_back(s);
            }
            if(j < NUM_PARTICLES - 1){
                Stick* s = new Stick(world->particles[i * NUM_PARTICLES + j], world->particles[i * NUM_PARTICLES + j + 1], 1, 1.5);
                world->sticks.push_back(s);
            }
        }
    }

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = true;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    running = true;
                    t1 = std::thread(cutSticks, world);
                    t1.detach();
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    running = false;
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        world->update(renderer);

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
