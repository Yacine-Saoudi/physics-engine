#include <iostream>
#include <cmath>
#include <vector>
#include <mutex>
#include <thread>
#include <SDL2/SDL.h>
#include "PhysicsWorld.hpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const double BOUNCE = 0.8;
const double AIR_RESISTANCE = 0.8;
const double GRAVITY = 0.01;
bool running = false;
std::thread t1;

void spawnParticles(PhysicsWorld* world){
    int mouse_x, mouse_y;
    float mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
    while(running){
        this_thread::sleep_for(chrono::milliseconds(300));
        if(!running) return;
        float mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
        Vec2 pos = Vec2(mouse_x, mouse_y);
        world->particles.push_back(new Particle(pos, pos, Vec2(0, 0), 5));
    }
    return;
}

int main(int argc, char* argv[]) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("PhysEng", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    // initialize physics world
    PhysicsWorld* world = new PhysicsWorld(SCREEN_HEIGHT, SCREEN_WIDTH, BOUNCE, AIR_RESISTANCE, GRAVITY);

    // create array of 5 particles array in the shape of a cube
    std::vector<Particle*> particles;
    std::vector<Stick*> sticks;

    Particle* p1 = new Particle(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), Vec2(0, 0), 5);
    Particle* p2 = new Particle(Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2), Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2), Vec2(0, 0), 5);
    Particle* p3 = new Particle(Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 + 50), Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 + 50), Vec2(0, 0), 5);
    Particle* p4 = new Particle(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 50), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 50), Vec2(0, 0), 5);
    world->particles.push_back(p1);
    world->particles.push_back(p2);
    world->particles.push_back(p3);
    world->particles.push_back(p4);

    Stick* s1 = new Stick(p1, p2, 1.0f);
    Stick* s2 = new Stick(p2, p3, 1.0f);
    Stick* s3 = new Stick(p3, p4, 1.0f);
    Stick* s4 = new Stick(p4, p1, 1.0f);
    Stick* s5 = new Stick(p1, p3, 1.0f);
    world->sticks.push_back(s1);
    world->sticks.push_back(s2);
    world->sticks.push_back(s3);
    world->sticks.push_back(s4);
    world->sticks.push_back(s5);

    // Event loop
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
                    t1 = std::thread(spawnParticles, world);
                    t1.detach();
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    running = false;
                    break;
                }
            }
        }
        // clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        world->updateParticles();
        world->updateSticks();
        world->constrainPoints();
        world->renderParticles(renderer);
        world->renderSticks(renderer);

        SDL_RenderPresent(renderer);
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
