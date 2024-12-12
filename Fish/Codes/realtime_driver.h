#include <bits/types/struct_sched_param.h>
#include <ctime>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>

typedef struct timespec tspec;

typedef struct task_parameters {
    long period;         // in ns
    long deadline;       // in ns
    tspec at;            // NEXT arrival time (absolute)
    tspec dl;            // CURRENT deadline (absolute). The next AT is the current deadline.
    int dmiss;
} task_parameters;

typedef struct task_args {
    task_parameters tp;
    void* args;
} task_args;


const long NANOSECONDS_IN_SECOND = 1000000000;

void insert_periodic_task(void* function(void*), void* arg);

// initialize scheduler
void init();

void time_copy(tspec *td, tspec* ts);
void add_ns(tspec *t, long ns);
void set_period(task_parameters *tp);
void wait_for_period(task_parameters *tp);
bool deadline_miss(task_parameters *tp);
void end_scheduler();
