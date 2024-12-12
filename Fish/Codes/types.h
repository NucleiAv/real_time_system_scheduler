#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/mouse.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float FPS = 60; // the simulation speed, or the rate at which timer events are set off.
const int NUM_PARTICLES = 100;
const float MAX_SPEED = 1.0;
const float NEIGHBOR_RADIUS = 50.0;
const float SEPARATION_RADIUS = 25.0;
const float PREDATOR_RADIUS = 100.0;
const float LEADER_PROBABILITY = 0.05;

struct Vector2 {
    float x, y;

    Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
    Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
    Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
    
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    float length() const { return std::sqrt(x * x + y * y); }
    Vector2 normalize() const { float len = length(); return len > 0 ? Vector2{x / len, y / len} : Vector2{0, 0}; }
    Vector2 rotate(Vector2 other) {
        Vector2 oNorm = other.normalize();
        float xPrime = x * oNorm.x - y * oNorm.y;
        float yPrime = x * oNorm.y + y * oNorm.x;
        return {xPrime, yPrime};
    };
};

class Particle {
public:
    Vector2 position;
    Vector2 velocity;
    bool is_leader;

    Particle(float x, float y, bool leader = false);
    void move(std::vector<Particle>& particles, Vector2 predator_position);

    void draw() const;
};

class Predator {
public:
    Vector2 position;

    Predator();
    void update(Vector2 mouse_pos);
    void draw() const;
};
