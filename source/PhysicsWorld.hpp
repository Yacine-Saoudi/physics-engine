#include "Stick.hpp"

using namespace std;
class PhysicsWorld
{
private:
    const int screen_width = 1280;
    const int screen_height = 720;
    const double bounce = 0.8;
    const double air_resistance = 0.8;
    const double gravity = 0.01;
    std::thread thread;
    bool running = false;

    vector<Particle*> particles;
    vector<Stick*> sticks;
public:
    PhysicsWorld(/* args */);
    ~PhysicsWorld();
    
    void updateParticles(vector<Particle*> particles);
    void updateSticks(vector<Stick*> sticks);
    void constrainPoints(vector<Particle*> particles);
    void renderParticles(SDL_Renderer* renderer, vector<Particle*> particles);
    void renderSticks(SDL_Renderer* renderer, vector<Stick*> sticks);
    void spawnParticles(vector<Particle*>* particles);

};

PhysicsWorld::PhysicsWorld(){
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::spawnParticles(vector<Particle*>* particles){
    int mouse_x, mouse_y;
    float mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
    while(running){
        this_thread::sleep_for(chrono::milliseconds(500));
        float mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
        Vec2 pos = Vec2(mouse_x, mouse_y);
        particles->push_back(new Particle(pos, pos, Vec2(0, 0), 5));
    }
}

void PhysicsWorld::renderSticks(SDL_Renderer* renderer, vector<Stick*> sticks) {
    // render the circle
    for (int i = 0; i < sticks.size(); i++) {
        Stick* s = sticks[i];
        s->render(renderer);
    }
}

void renderParticles(SDL_Renderer* renderer, std::vector<Particle*> particles) {
    // render the circle
    for (int i = 0; i < particles.size(); i++) {
        Particle* p = particles[i];
        p->render(renderer);
    }
}

void updateSticks(vector<Stick*> sticks) {
    for(int i = 0; i < sticks.size(); i++){
        Stick* s = sticks[i];
        s->update();
    }
}

void PhysicsWorld::constrainPoints(vector<Particle*> particles) {
        for (int i = 0; i < particles.size(); i++) {
            Particle* p = particles[i];
            p->constrainPoint(screen_height, screen_width, bounce, air_resistance);      
    }
}

void PhysicsWorld::updateParticles(vector<Particle*> particles) {
    // update the position of the circle using verlet integration
    for (int i = 0; i < particles.size(); i++) {    
        if(particles[i]->pinned) continue;
        Particle* p = particles[i];
        p->update(screen_height, gravity);

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
