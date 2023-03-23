#include <vector>
#include "Particle.hpp"

class Stick
{
public:    
    Particle* p1;
    Particle* p2;
    float length;
    float rigidity;
    float damping;

    Stick();
    Stick(Particle*, Particle*, float, float);
    ~Stick();
    
    float getLength();
    void updateLength();
    void render(SDL_Renderer*);
    void update();
    bool isTouching(Vec2);
    void resolveCollision(Particle*);
    
};

Stick::Stick(){
    Particle* p1 = nullptr;
    Particle* p2 = nullptr;
    length = 0;
    rigidity = 1;
    damping = 1;
}

Stick::Stick(Particle* p1, Particle* p2, float r, float d){
    this->p1 = p1;
    this->p2 = p2;
    this->length = sqrt(pow(p1->pos.x - p2->pos.x, 2) + pow(p1->pos.y - p2->pos.y, 2));
    this->rigidity = r;
    this->damping = d;
}

Stick::~Stick(){
}

/**
 * @brief check if a given position is touching this stick object
 * 
 * @param p the position to check
 * @return true if the position is touching the stick
 * @return false if the position is not touching the stick
 */
bool Stick::isTouching(Vec2 p){
    float radius = 3.0f;
    Vec2 topleft = Vec2(fmax(p1->pos.x, p2->pos.x), fmax(p1->pos.y, p2->pos.y));
    Vec2 bottomright = Vec2(fmin(p1->pos.x, p2->pos.x), fmin(p1->pos.y, p2->pos.y));

    if(p.x > topleft.x + radius || p.x < bottomright.x - radius || p.y > topleft.y + radius || p.y < bottomright.y - radius){
        return false;
    }
    
    if(p.y <= fmax(p1->pos.y, p2->pos.y) + radius && p.y >= fmin(p1->pos.y, p2->pos.y) - radius){
        return true;
    }
    return false;
}

void Stick::resolveCollision(Particle* p){
    // possible bug: the way that the newposition is calc'd after edge collision is not fully stable and can
    // cause a rounding error which creates a minute velocity when it is stationary
    float d1 = (p1->pos - p->pos).magnitude();
    float d2 = (p2->pos - p->pos).magnitude();
    float total = d1 + d2;
    float p1_percent = d1 / total;
    float p2_percent = d2 / total;


    // Calculate the velocity of the particle
    Vec2 particle_velocity = (p->pos - p->oldpos);
    
    // Calculate the velocity change for each particle
    Vec2 p1Vel = p1->pos - p1->oldpos;
    Vec2 p2Vel = p2->pos - p2->oldpos;
    Vec2 stick_velocity = (p1Vel + p2Vel) / 2.0f;
    Vec2 velocity_change = (stick_velocity - particle_velocity) * rigidity * damping;

    // if stick velocity is 0, then the stick is not moving and the particle's acceleration should 
    if(stick_velocity.magnitude() <= 0.1) return;

    // if the particle is pinned, apply the velocity change to the stick
    if(p->pinned){
        p1->pos -= velocity_change * p1_percent;
        p2->pos -= velocity_change * p2_percent;
        return;
    }
    

    // Apply the velocity change to the particle
    p->oldpos = p->pos;
    p->pos += velocity_change;

    // Apply the same velocity change to the stick
    p1->pos -= velocity_change * p1_percent;
    p2->pos -= velocity_change * p2_percent;
}

/**
 * @brief get the length of the stick
 * 
 * @return float value of the length of the stick
 */
float Stick::getLength(){
    return length;
}

/**
 * @brief change the length of the stick
 * 
 */
void Stick::updateLength(){
    float distance = sqrt(pow(p1->pos.x - p2->pos.x, 2) + pow(p1->pos.y - p2->pos.y, 2));
    this->length = distance;
}

/**
 * @brief render the stick between the two points if it's not hidden and both points are existing
 * 
 * @param renderer 
 */
void Stick::render(SDL_Renderer* renderer){
    if(p1 == nullptr || p2 == nullptr) return;
    if(p1->hidden || p2->hidden) return;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, p1->pos.x, p1->pos.y, p2->pos.x, p2->pos.y);
}

/**
 * @brief update the position of the stick and correct and change in distance that occured during the points' movement, selectively apply on non pinned points
 * 
 */
void Stick::update(){
    if(p1 == nullptr || p2 == nullptr) return;
    if(p1->pinned && p2->pinned) return;

    float dx = p1->pos.x - p2->pos.x,
            dy = p1->pos.y - p2->pos.y,
            distance = sqrt(dx * dx + dy * dy),
            diff = (length - distance) / distance * rigidity * damping,
            offsetX = dx * diff * 0.5,
            offsetY = dy * diff * 0.5,
            m1 = p1->mass + p2->mass,
            m2 = p1->mass / m1;
    m1 = p2->mass / m1;

    if(!p1->pinned){
        p1->pos.x += offsetX * m1;
        p1->pos.y += offsetY * m1;
    }

    if(!p2->pinned){
        p2->pos.x -= offsetX * m2;
        p2->pos.y -= offsetY * m2;
    }
}
