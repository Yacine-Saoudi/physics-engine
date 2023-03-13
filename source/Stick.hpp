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

bool Stick::isCollided(Particle* p){
    // if particle is outside of line segment
    if(p->pos.x < fmin(p1->pos.x, p2->pos.x) - p->radius || p->pos.x > fmax(p1->pos.x, p2->pos.x) + p->radius) return false;
    float d = fabs( (p2->pos - p->pos).cross(p1->pos - p->pos)) / fabs((p2->pos - p1->pos).magnitude());
    return d <= p->radius;
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
