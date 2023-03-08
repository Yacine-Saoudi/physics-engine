#include <iostream>
#include <cmath>
#include <vector>
#include <mutex>
#include <thread>
#include <SDL2/SDL.h>
#include "Stick.hpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const double BOUNCE = 0.8;
const double AIR_RESISTANCE = 0.8;
const double GRAVITY = 0.01;
std::thread thread;
bool running = false;

std::mutex mutex;

void updateParticles(std::vector<Particle*> particles) {
    // update the position of the circle using verlet integration
    for (int i = 0; i < particles.size(); i++) {    
        if(particles[i]->pinned) continue;
        Particle* p = particles[i];
        p->update(SCREEN_HEIGHT, GRAVITY);

    }
    // for (int i = 0; i < CURR_PARTICLES; i++){
    //     for(int j = i + 1; j < CURR_PARTICLES; j++){
    //         if(isCollided(particles[i], entities[j])){
    //             std::cout << "collision between " << i << " and " << j << std::endl;
    //             // collision response
    //             float dx = particles[i].pos.x - entities[j].pos.x;
    //             float dy = particles[i].pos.y - entities[j].pos.y;
    //             float distance = sqrt(dx * dx + dy * dy);
    //             float overlap = (particles[i].radius + entities[j].radius) - distance;

    //             Vec2 normal = Vec2(dx/distance, dy/distance);

    //             particles[i].pos.x += normal.x * (overlap / 2);
    //             particles[i].pos.y += normal.y * (overlap / 2);
    //             entities[j].pos.x -= normal.x * (overlap / 2);
    //             entities[j].pos.y -= normal.y * (overlap / 2);
    //         }
    //     }
    // }

}

void constrainPoints(std::vector<Particle*> particles) {
        for (int i = 0; i < particles.size(); i++) {
            Particle* p = particles[i];
            p->constrainPoint(SCREEN_HEIGHT, SCREEN_WIDTH, BOUNCE, AIR_RESISTANCE);      
    }
}

void updateSticks(std::vector<Stick*> sticks) {
    for(int i = 0; i < sticks.size(); i++){
        Stick* s = sticks[i];
        s->update();
    }
}

void renderParticles(SDL_Renderer* renderer, std::vector<Particle*> particles) {
    // render circles as white
    for (int i = 0; i < particles.size(); i++) {
        Particle* p = particles[i];
        p->render(renderer);
    }
}

void renderSticks(SDL_Renderer* renderer, std::vector<Stick*> sticks) {
    // render sticks as white
    for(int i = 0; i < sticks.size(); i++){
        Stick* s = sticks[i];
        s->render(renderer);
    }
}

void spawnParticles(std::vector<Particle*>* particles){
    std::lock_guard<std::mutex> guard(mutex);

    int mouseX, mouseY;
    float mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    while(running){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        float mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        if(mouseState == 0) continue;
        Vec2 pos = Vec2(mouseX, mouseY);
        particles->push_back(new Particle(pos, pos, Vec2(0, 0), 5));
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

    // create array of 5 particles array in the shape of a cube
    std::vector<Particle*> particles;
    std::vector<Stick*> sticks;

    Particle* p1 = new Particle(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), Vec2(0, 0), 5);
    Particle* p2 = new Particle(Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2), Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2), Vec2(0, 0), 5);
    Particle* p3 = new Particle(Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 + 50), Vec2(SCREEN_WIDTH/2 + 50, SCREEN_HEIGHT/2 + 50), Vec2(0, 0), 5);
    Particle* p4 = new Particle(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 50), Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 50), Vec2(0, 0), 5);
    particles.push_back(p1);
    particles.push_back(p2);
    particles.push_back(p3);
    particles.push_back(p4);

    Stick* s1 = new Stick(p1, p2, 1.0f);
    Stick* s2 = new Stick(p2, p3, 1.0f);
    Stick* s3 = new Stick(p3, p4, 1.0f);
    Stick* s4 = new Stick(p4, p1, 1.0f);
    Stick* s5 = new Stick(p1, p3, 1.0f);
    sticks.push_back(s1);
    sticks.push_back(s2);
    sticks.push_back(s3);
    sticks.push_back(s4);
    sticks.push_back(s5);

    // Event loop
    bool quit = false;
    bool mouseDown = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = true;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    std::cout << "click" << std::endl;
                    running = true;
                    thread = std::thread(spawnParticles, &particles);
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    std::cout << "unclick" << std::endl;
                    running = false;
                    std::cout << "running is false" << std::endl;
                    break;
                }
            }
        }

        // if running is false and thread is joinable, join the thread
        if(!running && thread.joinable()){
            std::cout << "joining thread" << std::endl;
            thread.join();
        }

        // clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // update the position of the circle
        updateParticles(particles);

        // if(mouseDown){
        //     std::cout << "mouse down" << std::endl;
        //     // std::vector<Particle*>* temp = &particles;
        //     // spawnThread = std::thread(spawnParticles, temp);
        //     spawnParticles(&particles);
        //     // sleep
        //     SDL_Delay(100);
        // }


        // update the position of the sticks
        updateSticks(sticks);
        // keep points within the screen
        constrainPoints(particles);

        // // render circles
        renderParticles(renderer, particles);

        renderSticks(renderer, sticks);

        // Present the renderer to the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
