#include "Stick.hpp"

using namespace std;
class PhysicsWorld
{
private:
    int screen_width;
    int screen_height;
    double bounce;
    double air_resistance;
    double gravity;
public:

    vector<Particle*> particles;
    vector<Stick*> sticks;

    PhysicsWorld(int, int, double, double, double);
    PhysicsWorld();
    ~PhysicsWorld();
    
    void updateParticles();
    void updateSticks();
    void constrainPoints();
    void renderParticles(SDL_Renderer*);
    void renderSticks(SDL_Renderer*);
    void update(SDL_Renderer*);
    void cutSticks(Vec2);

};

/**
 * @brief Construct a new Physics World:: Physics World object
 * 
 * @param s_h screen height of the window
 * @param s_w screen width of the window
 * @param b bounce coefficient of the simulation
 * @param a_r air resistence coefficient of the simulation
 * @param g gravity value of the simulation
 */
PhysicsWorld::PhysicsWorld(int s_h, int s_w, double b, double a_r, double g){
    screen_height = s_h;
    screen_width = s_w;
    bounce = b;
    air_resistance = a_r;
    gravity = g;
}

/**
 * @brief Construct a new Physics World:: Physics World object
 * 
 */
PhysicsWorld::PhysicsWorld(){
    screen_width = 1280;
    screen_height = 720;
    bounce = 0.8;
    air_resistance = 0.8;
    gravity = 0.01;
}


/**
 * @brief Destroy the Physics World:: Physics World object
 * 
 */
PhysicsWorld::~PhysicsWorld(){
}

/**
 * @brief check if a given position is touching any of the sticks in the sticks array and erase the stick if it is
 * 
 * @param pos position to check
 */
void PhysicsWorld::cutSticks(Vec2 pos){
    for(int i = 0; i < sticks.size(); i++){
        if(sticks[i]->isTouching(pos)){
            sticks.erase(sticks.begin() + i);
        }
    }
}

/**
 * @brief iterate through the sticks array and apply the render function to each stick
 * 
 * @param renderer SDL_renderer to draw sticks in
 */
void PhysicsWorld::renderSticks(SDL_Renderer* renderer) {
    for (int i = 0; i < sticks.size(); i++) {
        Stick* s = sticks[i];
        s->render(renderer);
    }
}
/**
 * @brief iterate through the particles array and render each particle
 * 
 * @param renderer SDL_renderer to draw particles in
 */
void PhysicsWorld::renderParticles(SDL_Renderer* renderer) {
    for (int i = 0; i < particles.size(); i++) {
        Particle* p = particles[i];
        p->render(renderer);
    }
}

/**
 * @brief iterate through sticks array and update the position of each stick
 * 
 */
void PhysicsWorld::updateSticks() {
    for(int i = 0; i < sticks.size(); i++){
        Stick* s = sticks[i];
        s->update();
    }
}

/**
 * @brief iterate through each particle in the particles array and constrain it within the boundary of the window
 * 
 */
void PhysicsWorld::constrainPoints() {
        for (int i = 0; i < particles.size(); i++) {
            Particle* p = particles[i];
            p->constrainPoint(screen_height, screen_width, bounce, air_resistance);      
    }
}

/**
 * @brief iterate through the particles array and update the position of each particle
 * 
 */
void PhysicsWorld::updateParticles() {
    //TODO: apply collision resolution and detection
    // update the position of the circle using verlet integration
    for (int i = 0; i < particles.size(); i++) {    
        if(particles[i]->pinned) continue;
        Particle* p = particles[i];
        p->update(screen_height, gravity);

    }
    for (int i = 0; i < particles.size(); i++){
        for(int j = i + 1; j < particles.size(); j++){
            if(particles[i]->isCollided(particles[j])){
                particles[i]->resolveCollision(particles[j]);
            }
        }
    }

}

/**
 * @brief update all physics world objects and render them
 * 
 * @param renderer renderer to draw objects in
 */
void PhysicsWorld::update(SDL_Renderer* renderer) {
    updateParticles();
    updateSticks();
    constrainPoints();
    renderParticles(renderer);
    renderSticks(renderer);
}