#include <vector>
#include "Particle.hpp"

class Stick
{
public:    
    Particle* p1;
    Particle* p2;
    float length;
    float rigidity;

    Stick();
    Stick(Particle*, Particle*, float);
    ~Stick();
    
    float getLength();
    void updateLength();
    void render(SDL_Renderer*);
    void update();
    
};

Stick::Stick(){
    Particle* p1 = nullptr;
    Particle* p2 = nullptr;
    length = 0;
    rigidity = 0;
}

Stick::Stick(Particle* p1, Particle* p2, float r){
    this->p1 = p1;
    this->p2 = p2;
    this->length = sqrt(pow(p1->pos.x - p2->pos.x, 2) + pow(p1->pos.y - p2->pos.y, 2));
    this->rigidity = r;
}

Stick::~Stick(){
}

float Stick::getLength(){
    return length;
}

void Stick::updateLength(){
    float distance = sqrt(pow(p1->pos.x - p2->pos.x, 2) + pow(p1->pos.y - p2->pos.y, 2));
    this->length = distance;
}

void Stick::render(SDL_Renderer* renderer){
    if(p1 == nullptr || p2 == nullptr) return;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, p1->pos.x, p1->pos.y, p2->pos.x, p2->pos.y);
}

void Stick::update(){
    float dx = p1->pos.x - p2->pos.x,
            dy = p1->pos.y - p2->pos.y,
            distance = sqrt(dx * dx + dy * dy),
            diff = (length - distance) / distance * rigidity,
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
