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
    bool isCollided(Particle*);
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
 * @brief check if the stick is colliding with the given particle
 * 
 * @param p particle to check against
 * @return true if particle is collided with stick
 * @return false if particle is attached to stick or outside of stick's line segment or not colliding with stick
 */
bool Stick::isCollided(Particle* p){
    // if particle is outside of line segment
    if(p == p1 || p == p2) return false;
    if(p->pos.x < fmin(p1->pos.x, p2->pos.x) - p->radius || p->pos.x > fmax(p1->pos.x, p2->pos.x) + p->radius) return false;
    // if particle is not touching the line segment
    if(p->pos.y < fmin(p1->pos.y, p2->pos.y) - p->radius || p->pos.y > fmax(p1->pos.y, p2->pos.y) + p->radius) return false;

    // if particle is not colliding with the line segment
    float d = abs((p2->pos.y - p1->pos.y) * p->pos.x - (p2->pos.x - p1->pos.x) * p->pos.y + p2->pos.x * p1->pos.y - p2->pos.y * p1->pos.x) / sqrt(pow(p2->pos.y - p1->pos.y, 2) + pow(p2->pos.x - p1->pos.x, 2));
    // this works by calculating the distance from the particle to the line segment and checking if it is less than the radius of the particle
    // if it is, then the particle is colliding with the line segment
    // it divides the line segment into two triangles and calculates the distance from the particle to the line segment by calculating the distance from the particle to the hypotenuse of the triangle

    return d <= p->radius;
}

void Stick::resolveCollision(Particle* p){
    // combined velocity
    // possible bug: the way that the newposition is calc'd after edge collision is not fully stable and can
    // cause a rounding error which creates a minute velocity when it is stationary
    // calculate the percentage of the force applied to which particle depending on the distance from the center of the stick
    float d1 = (p1->pos - p->pos).magnitude();
    float d2 = (p2->pos - p->pos).magnitude();
    float total = d1 + d2;
    float p1_percent = d1 / total;
    float p2_percent = d2 / total;


    // Calculate the velocity of the particle
    Vec2 particle_velocity = (p->pos - p->oldpos);

    std::cout << "particle velocity: " << particle_velocity.x << ", " << particle_velocity.y << std::endl;

    // Calculate the velocity change for each particle
    Vec2 p1Vel = p1->pos - p1->oldpos;
    Vec2 p2Vel = p2->pos - p2->oldpos;
    Vec2 stick_velocity = (p1Vel + p2Vel) / 2.0f;
    Vec2 velocity_change = (stick_velocity - particle_velocity) * rigidity * damping;

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
