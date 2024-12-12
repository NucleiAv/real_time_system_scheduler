// everything will be scheduled with SCHED_FIFO.
// every thread has the same priority.
// 
// tasks:
// - mouse input
//    - move predator (update position)
// - move fish
//    - one thread per fish
// - display
//    - draw predator
//    - draw fish
//    - clear display
//    - flip display
//

#include "realtime_driver.h"
#include <ctime>
#include <iostream>
#include <sched.h>


std::vector<pthread_t> THREADS;
pthread_attr_t SCHEDULER_ATTRIBUTES;
struct sched_param SCHEDULER_PARAMETERS;

void insert_periodic_task(void* func(void*), void* args){
    pthread_t thread;

    // Create threads for each task
    pthread_create(&thread, &SCHEDULER_ATTRIBUTES, func, args);
    THREADS.push_back(thread);
}

// join all threads (end execution)
void end_scheduler() {
    std::cout << "END_SCHEDULER ENTER" << std::endl;
    for(pthread_t& p: THREADS) {
        std::cout << "AWAITING THREAD: " << p << std::endl;
        pthread_join(p, NULL);
    }
    std::cout << "END_SCHEDULER EXIT" << std::endl;
}

// initialize scheduler
void init() {
    // setup scheduler
    pthread_attr_init(&SCHEDULER_ATTRIBUTES);
    pthread_attr_setinheritsched(&SCHEDULER_ATTRIBUTES, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&SCHEDULER_ATTRIBUTES, SCHED_FIFO);
    
    // include schedule priority
    SCHEDULER_PARAMETERS.sched_priority = 10;
    pthread_attr_setschedparam(&SCHEDULER_ATTRIBUTES, &SCHEDULER_PARAMETERS);
}

void time_copy(tspec *td, tspec* ts) {
    td->tv_sec = ts->tv_sec;
    td->tv_nsec = ts->tv_nsec;
}

bool time_cmp(tspec a, tspec b) {
  if (a.tv_sec == b.tv_sec) {
    return a.tv_nsec > b.tv_nsec;
  }
  return a.tv_sec > b.tv_sec;
}

void add_ns(tspec *t, long ns) {
    int units = (int)(ns/NANOSECONDS_IN_SECOND);
    t->tv_nsec += ns - units * NANOSECONDS_IN_SECOND;
    t->tv_sec += units;
    if (t->tv_nsec > NANOSECONDS_IN_SECOND) {
        t->tv_nsec -= NANOSECONDS_IN_SECOND;
       t->tv_sec += 1;
    }
}

void set_period(task_parameters *tp) {
    tspec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    time_copy(&(tp->at),&t);
    time_copy(&(tp->dl),&t);
    add_ns(&(tp->at), tp->period);
    add_ns(&(tp->dl), tp->deadline);   
    tp->dmiss = 0;
}

void wait_for_period(task_parameters *tp) {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(tp->at), NULL);
    add_ns(&(tp->at), tp->period);
    add_ns(&(tp->dl), tp->period);
}

bool deadline_miss(task_parameters *tp) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  if (time_cmp(now, (tp->dl)) > 0) {
    tp->dmiss++;
    return true;
  }
  return false;
}
