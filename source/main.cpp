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

    // create particles in the shape of a box to test if collision detection between stick and particle works
    Particle* pone = new Particle(Vec2(100, 100), Vec2(100, 100), Vec2(0, 0), 5);
    Particle* ptwo = new Particle(Vec2(200, 200), Vec2(200, 200), Vec2(0, 0), 5);
    Particle* pthree = new Particle(Vec2(100, 200), Vec2(100, 200), Vec2(0, 0), 5);
    Particle* pfour = new Particle(Vec2(200, 100), Vec2(200, 100), Vec2(0, 0), 5);
    Particle* pfive = new Particle(Vec2(150, 150), Vec2(150, 150), Vec2(0, 0), 5);

    world->particles.push_back(pone);
    world->particles.push_back(ptwo);
    world->particles.push_back(pthree);
    world->particles.push_back(pfour);

    Particle* pintersect = new Particle(Vec2(150, 150), Vec2(150, 150), Vec2(0, 0), 5);
    world->particles.push_back(pintersect);

    Particle* pintersectcorner = new Particle(Vec2(100, 100), Vec2(100, 100), Vec2(0, 0), 5);
    world->particles.push_back(pintersectcorner);

    Particle* notintersect = new Particle(Vec2(300, 300), Vec2(300, 300), Vec2(0, 0), 5);
    world->particles.push_back(notintersect);

    // create a stick between the two particles
    Stick* stickone = new Stick(pone, ptwo, 0.1, 0.8);
    Stick* sticktwo = new Stick(ptwo, pthree, 0.1, 0.8);
    Stick* stickthree = new Stick(pthree, pfour, 0.1, 0.8);
    Stick* stickfour = new Stick(pfour, pone, 0.1, 0.8);
    Stick* stickfive = new Stick(pone, pthree, 0.1, 0.8);
    Stick* sticksix = new Stick(ptwo, pfour, 0.1, 0.8);
    
    world->sticks.push_back(stickone);
    world->sticks.push_back(sticktwo);
    world->sticks.push_back(stickthree);
    world->sticks.push_back(stickfour);
    world->sticks.push_back(stickfive);
    world->sticks.push_back(sticksix);
    

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

        // std::cout << "isCollided: " << stick->isCollided(pintersect) << std::endl;
        // std::cout << "corner: " << stick->isCollided(pintersectcorner) << std::endl;
        // std::cout << "not: " << stick->isCollided(notintersect) << std::endl;


        world->update(renderer);

        SDL_RenderPresent(renderer);
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
