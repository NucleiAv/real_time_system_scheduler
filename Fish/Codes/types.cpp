#include "types.h"
#include <allegro5/color.h>

Particle::Particle(float x, float y, bool leader) : is_leader(leader) {
    position = {x, y};
    velocity = {(float)(std::rand() % 100 / 50.0 - 1), (float)(std::rand() % 100 / 50.0 - 1)};
}

void Particle::move(std::vector<Particle>& particles, Vector2 predator_position) {
    Vector2 alignment = {0, 0};
    Vector2 cohesion = {0, 0};
    Vector2 separation = {0, 0};
    int count = 0;
    Particle* leader = nullptr;

    for (Particle& other : particles) {
        if (&other != this) {
            float distance = (other.position - position).length();

            if (distance < NEIGHBOR_RADIUS) {
                if (other.is_leader) {
                    leader = &other;
                }
                alignment += other.velocity;
                cohesion += other.position;
                count++;
            }

            if (distance < SEPARATION_RADIUS) {
                separation += (position - other.position).normalize() * (1.0f / distance);
            }
        }
    }

    if (count > 0) {
        alignment = (alignment * (1.0f / count)).normalize() * 0.5f;
        cohesion = ((cohesion * (1.0f / count)) - position).normalize() * 0.05f;
        separation = separation * 0.5f;
    }

    Vector2 flee = {0, 0};
    if ((predator_position - position).length() < PREDATOR_RADIUS) {
        flee = (position - predator_position).normalize() * 1.0f;
    } else if (leader && !is_leader) {
        Vector2 follow_leader = (leader->position - position).normalize() * 0.1f;
        velocity += follow_leader;
    }

    velocity += alignment + cohesion + separation + flee;

    if (velocity.length() > MAX_SPEED) {
        velocity = velocity.normalize() * MAX_SPEED;
    }

    position += velocity;

    // Wrap around screen edges
    if (position.x < 0) position.x = SCREEN_WIDTH;
    if (position.x > SCREEN_WIDTH) position.x = 0;
    if (position.y < 0) position.y = SCREEN_HEIGHT;
    if (position.y > SCREEN_HEIGHT) position.y = 0;
}

void Particle::draw() const {
    float size = 10;
    float thickness = 3;
    ALLEGRO_COLOR color = al_map_rgb(is_leader ? 0: 225, is_leader ? 225: 0, 0);

    Vector2 leftFront = (Vector2({-1, -4}).normalize() * size).rotate(velocity);
    Vector2 rightFront = (Vector2({-1, +4}).normalize() * size).rotate(velocity);
    Vector2 leftBack = (Vector2({-2, -1}).normalize() * size*2).rotate(velocity);
    Vector2 rightBack = (Vector2({-2, +1}).normalize() * size*2).rotate(velocity);
    //al_draw_filled_circle(position.x, position.y, 5, al_map_rgb(255, 255, 255));
    //direction line
    al_draw_line(position.x, position.y, position.x + velocity.normalize().x * size, position.y + velocity.normalize().y * size, color, thickness);
    //left-front
    al_draw_line(position.x + leftFront.x, position.y + leftFront.y, position.x + velocity.normalize().x * size, position.y + velocity.normalize().y * size, color, thickness);
    //right-front
    al_draw_line(position.x + rightFront.x, position.y + rightFront.y, position.x + velocity.normalize().x * size, position.y + velocity.normalize().y * size, color, thickness);
    //left-back
    al_draw_line(position.x + leftBack.x, position.y + leftBack.y, position.x + rightFront.x, position.y + rightFront.y, color, thickness);
    //right-back
    al_draw_line(position.x + rightBack.x, position.y + rightBack.y, position.x + leftFront.x, position.y + leftFront.y, color, thickness);
    //tail
    al_draw_line(position.x + rightBack.x, position.y + rightBack.y, position.x + leftBack.x, position.y + leftBack.y, color, thickness);
}

Predator::Predator() : position({0, 0}) {}

void Predator::update(Vector2 mouse_pos) {
    position = mouse_pos;
}

void Predator::draw() const {
    al_draw_filled_circle(position.x, position.y, 10, al_map_rgb(255, 0, 0));
}
