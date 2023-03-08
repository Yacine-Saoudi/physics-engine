#include "Vec2.hpp"

class Particle
{
public:
    Vec2 pos;
    Vec2 oldpos;
    Vec2 acc;
    float mass;
    float radius;
    bool pinned;
    bool hidden;

    Particle();
    Particle(Vec2, Vec2, Vec2, float);
    ~Particle();

    void render(SDL_Renderer*);
    void update(const int, const float);
    bool isCollided(Particle*);
    void constrainPoint(const int, const int, const float, const float);
};

Particle::Particle(){
    pos = Vec2(0, 0);
    oldpos = Vec2(0, 0);
    acc = Vec2(0, 0);
    mass = 1;
    radius = 5;
    pinned = false;
    hidden = false;
}

Particle::Particle(Vec2 p, Vec2 op, Vec2 a, float m){
    pos = p;
    oldpos = op;
    acc = a;
    mass = m;
    radius = 10;
    pinned = false;
    hidden = false;
}

Particle::~Particle()
{
}

void SDL_RenderDrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int r) {
    const int diameter = (r * 2);

    int x = (r - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);
    while(x >= y) {
        //  Each of the following renders an octant of the circle
        // have the circle be filled in
        SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX + x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX - x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX + y, centerY + x, centerX + y, centerY - x);
        SDL_RenderDrawLine(renderer, centerX - y, centerY + x, centerX - y, centerY - x);
        
        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void Particle::render(SDL_Renderer* renderer){
    if(!hidden){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawCircle(renderer, pos.x, pos.y, radius);
    }
}

void Particle::update(const int screen_height, const float gravity){
    if(!pinned){
        float vx = pos.x - oldpos.x,
              vy = pos.y - oldpos.y;
        
        oldpos.x = pos.x;
        oldpos.y = pos.y;

        pos.x += vx + acc.x;
        pos.y += vy + acc.y + gravity;

        if(pos.y + radius + vy >= screen_height){
            pos.x -= vx;
        }
    }
}

bool Particle::isCollided(Particle* other){
    float dx = pos.x - other->pos.x,
          dy = pos.y - other->pos.y,
          distance = sqrt(dx * dx + dy * dy);

    return distance < radius + other->radius;
}

void Particle::constrainPoint(const int screen_height, const int screen_width, const float bounce, const float air_friction){
    float vx = (pos.x - oldpos.x) * air_friction,
          vy = (pos.y - oldpos.y) * air_friction;
    if(pos.x + radius > screen_width){
        pos.x = screen_width - radius;
        oldpos.x = screen_width - radius + vx * bounce;
    } else if(pos.x - radius < 0){
        pos.x = radius;
        oldpos.x = pos.x + vx * bounce;
    }

    if(pos.y + radius > screen_height){
        pos.y = screen_height - radius;
        oldpos.y = screen_height - radius + vy * bounce;
    } else if(pos.y - radius < 0){
        pos.y = radius;
        oldpos.y = pos.y + vy * bounce;
    }
}