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
    void resolveCollision(Particle*);
};

/**
 * @brief Construct a new Particle:: Particle object
 * 
 * @param p the initial position of the particle
 * @param op the old position of the particle, on initialization this determines starting velocity
 * @param a the starting acceleration of the particle
 * @param m the mass of the particle
 */
Particle::Particle(Vec2 p, Vec2 op, Vec2 a, float m){
    pos = p;
    oldpos = op;
    acc = a;
    mass = m;
    radius = 1;
    pinned = false;
    hidden = false;
}

/**
 * @brief Construct a new Particle:: Particle object
 * 
 */
Particle::Particle(){
    pos = Vec2(0, 0);
    oldpos = Vec2(0, 0);
    acc = Vec2(0, 0);
    mass = 1;
    radius = 1;
    pinned = false;
    hidden = false;
}

/**
 * @brief Destroy the Particle:: Particle object
 * 
 */
Particle::~Particle()
{
}

/**
 * @brief take coordinates of the center of the circle and the radius and draw the circle in the SDL_Renderer
 * 
 * @param renderer SDL_Renderer to draw the circle to
 * @param centerX x coordinate of the circle's center
 * @param centerY y coordinate of the circle's center
 * @param r radius of the circle
 */
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

/**
 * @brief render the particle if it's not hidden
 * 
 * @param renderer SDL_Renderer to draw the particle to
 */
void Particle::render(SDL_Renderer* renderer){
    if(!hidden){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawCircle(renderer, pos.x, pos.y, radius);
    }
}


/**
 * @brief resolve the collision between two particles
 * 
 * @param p other particle to resolve collision with
 */
void Particle::resolveCollision(Particle* p){
    Vec2 relVel = Vec2(pos - p->pos);
    float minDist = radius + p->radius;
    float dist = sqrt(relVel.x * relVel.x + relVel.y * relVel.y);

    Vec2 normal = relVel.normalize();

    float mass_ratio_1 = radius / (radius + p->radius);
    float mass_ratio_2 = p->radius / (radius + p->radius);
    float delta = 0.5f * (dist - minDist);

    pos -= normal * (delta * mass_ratio_1);
    p->pos += normal * (delta * mass_ratio_2);
}

/**
 * @brief calculate the new position of the particle and apply acceleration and gravity, as well as ground friction
 * 
 * @param screen_height height of the screen
 * @param gravity gravity value to apply to the particle
 */
void Particle::update(const int screen_height, const float gravity){
    if(!pinned){
        float vx = pos.x - oldpos.x,
              vy = pos.y - oldpos.y;
                
        oldpos.x = pos.x;
        oldpos.y = pos.y;

        pos.x += vx + acc.x;
        pos.y += vy + acc.y + gravity;

    }
}

/**
 * @brief check if the particle is collided with another particle
 * 
 * @param other pointer to the other particle
 * @return true if the particles are colliding
 * @return false if the particles are not colliding
 */
bool Particle::isCollided(Particle* other){

    float dx = pos.x - other->pos.x,
          dy = pos.y - other->pos.y,
          distance = sqrt(dx * dx + dy * dy);

    return distance < radius + other->radius;
}

/**
 * @brief recalculate particle's trajectory and position to constrain it to the bounds of the window
 * 
 * @param screen_height height of the window
 * @param screen_width width of the window
 * @param bounce amount of kinetic energy lost to a bounce
 * @param air_friction background speed reduction of the particle as it travels
 */
void Particle::constrainPoint(const int screen_height, const int screen_width, const float bounce, const float air_friction){
    //TODO: fix and rework air_friction so that it actually does something
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