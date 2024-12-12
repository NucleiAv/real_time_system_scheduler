#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/display.h>
#include <allegro5/events.h>
#include <allegro5/mouse.h>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "realtime_driver.h"
#include "types.h"
#include <iostream>
#include <pthread.h>

std::vector<Particle> fishes;
Predator predator;

bool end = false;
bool wait_for_display = false;

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* event_queue;

// SYSTEM CONSTANTS      ns     us     ms
const long BASE_PERIOD = 1000 * 1000 * 10;       // the base period of a task

// TASK: detect mouse movement and update the predator's position, which will also affect the fishes
void* mouse_update(void* args) {
    
    ALLEGRO_MOUSE_STATE state;
    task_parameters tp = ((task_args*)args)->tp;
    set_period(&tp);
    
    while(!end) {
    if(!wait_for_display){
        al_get_mouse_state(&state);   
        predator.update({(float)state.x, (float)state.y});

        if(deadline_miss(&tp)){
            flockfile(stdout);
            std::cout << "UPDATE_MISS TOTAL: " << tp.dmiss << std::endl;
            funlockfile(stdout);
        }
    }
    wait_for_period(&tp);
    }
    flockfile(stdout);
    std::cout << "MOUSE_UPDATE_END" << std::endl;
    funlockfile(stdout);
    return nullptr;
}

// TASK: move fish based on current mouse location (regardless of update).
void* move_fish(void* args) {
    
    task_parameters tp = ((task_args*)args)->tp;
    set_period(&tp);
    Particle* fish = (Particle*)((task_args*)args)->args;
    
    while(!end) {
        fish->move(fishes, predator.position);
        
        if(deadline_miss(&tp)){
            flockfile(stdout);
            std::cout << "MOVE_MISS TOTAL: " << tp.dmiss << " FOR " << fish << std::endl;
            funlockfile(stdout);
        }
        wait_for_period(&tp);
    }
    flockfile(stdout);
    std::cout << "MOVE_FISH_END:" << fish << std::endl;
    funlockfile(stdout);
    return nullptr;
}

// TASK: Drawing thread. Clear the display buffer of the previous cycle and write changes for the 
// current cycle. Flip display
void* display_manager(void* args) {
    
    task_parameters tp = ((task_args*)args)->tp;
    al_init_primitives_addon();
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (display == nullptr) {
        std::cout << "DISPLAY CREATION FAILED" << std::endl;
        exit(1);
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));

    while(!end) {
        if(!wait_for_display) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            for(Particle& fish: fishes) fish.draw();
            predator.draw();
            al_flip_display();
            
            if(deadline_miss(&tp)){
                flockfile(stdout);
                //std::cout << "DISPLAY_MISS: " << tp.dmiss << std::endl;
                funlockfile(stdout);
            }
        }
        wait_for_period(&tp);
    }
    flockfile(stdout);
    std::cout << "DISPLAY_END" << std::endl;
    funlockfile(stdout);
    return nullptr;
}

int main() {
    std::srand(std::time(0));

    // initialize allegro
    al_init();
    al_install_mouse();
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);

    // create display and event_queue which will be used to hold the events to be run
    event_queue = al_create_event_queue();

    // POPULATE FISH: randomly select leaders with LEADER_PROBABILITY
    bool leader_exists; // ensure that at least ONE leader exists
    for (int i = 0; i < NUM_PARTICLES-1; ++i) {
        bool is_leader = ((std::rand() / (float)RAND_MAX) < LEADER_PROBABILITY) && (leader_exists = true);
        fishes.emplace_back(std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT, is_leader);
    }
    if (!leader_exists) fishes.emplace_back(std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT, true);

    // task parameters (same for every thread)
    task_parameters tp = {
        .period = BASE_PERIOD,
        .deadline = BASE_PERIOD,
    };

    // display & mouse tasks
    task_args display_args = {
        .tp = tp,
        .args = NULL
    };

    task_args mouse_args = {
        .tp = tp,
        .args = NULL
    };

    insert_periodic_task(display_manager, (void*)&display_args);
    insert_periodic_task(mouse_update, (void*)&mouse_args);

    // move all fishes
    for (Particle &fish: fishes) {
        task_args* args = (task_args*)malloc(sizeof(task_args));
        args->args = &fish;
        args->tp = tp;
        
        insert_periodic_task(move_fish, (void*)args);
    }

    ALLEGRO_EVENT event;
    while(!end) {
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            std::cout << "END SIGNALED" << std::endl;
            end = true;
            end_scheduler();
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT) wait_for_display = true;
        else if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN) wait_for_display = false;
    }
    // de-register display
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    // exit successfully
    return 0;
}

