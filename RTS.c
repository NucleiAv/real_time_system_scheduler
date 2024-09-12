#include <stdio.h>
#include <math.h>

#define MAX_TASKS 10

typedef struct {
    double Ci; // Computation Time
    double Ti; // Time Period
    double Di; // Relative Deadline
    int task_id; // Task ID for tracking
} Task;

void input_tasks(Task tasks[], int num) {
    printf("NOTE - For RM Algo, kindly make sure while entering the parameters that the Di = Ti. \n");
    printf("NOTE - Provide values of parameters with space b/w each. \n");
    for (int i = 0; i < num; i++) {
        printf("Enter Computation time (Ci), Relative deadline (Di), Time period (Ti) for Task %d: ", i + 1);
        scanf("%lf %lf %lf", &tasks[i].Ci, &tasks[i].Di, &tasks[i].Ti);
        tasks[i].task_id = i + 1;
    }
}

double utilization_factor(Task tasks[], int num) {
    double U = 0;
    for (int i = 0; i < num; i++) {
        U += tasks[i].Ci / tasks[i].Ti;
    }
    return U;
}

double liu_layland_bound(int num) {
    return num * (pow(2, 1.0 / num) - 1);
}

int hyperbolic_bound(Task tasks[], int num) {
    double product = 1.0;
    for (int i = 0; i < num; i++) {
        product *= (tasks[i].Ci / tasks[i].Ti + 1);
    }
    return (product <= 2);
}

// Function to sort tasks by Time Period for RM
void sort_by_period(Task tasks[], int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if (tasks[i].Ti > tasks[j].Ti) {
                Task temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }
}

// Function to sort tasks by Relative Deadline for DM
void sort_by_deadline(Task tasks[], int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if (tasks[i].Di > tasks[j].Di) {
                Task temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }
}

int response_time_analysis(Task tasks[], int num, int *order, int is_rm) {
    double response[MAX_TASKS];
    for (int i = 0; i < num; i++) {
        response[i] = tasks[i].Ci;
    }

    for (int i = 0; i < num; i++) {
        double R = tasks[i].Ci;
        while (1) {
            double W = tasks[i].Ci;
            for (int j = 0; j < i; j++) {
                W += ceil(R / tasks[j].Ti) * tasks[j].Ci;
            }
            if (W == R) {
                break;
            }
            R = W;
        }
        response[i] = R;

        if ((is_rm && R > tasks[i].Ti) || (!is_rm && R > tasks[i].Di)) {
            return 0; // Not schedulable
        }
        order[i] = tasks[i].task_id; // Assign task execution order
    }
    return 1; // Schedulable
}

int processor_demand_analysis(Task tasks[], int num, int *order) {
    for (double L = 0; L <= tasks[num - 1].Ti; L += 0.1) { // Iterate up to the largest time period
        double demand = 0;
        for (int i = 0; i < num; i++) {
            demand += ceil(L / tasks[i].Ti) * tasks[i].Ci;
        }
        if (demand > L) {
            return 0; // Not schedulable
        }
    }
    // If PDA succeeds, generate order (for EDF, tasks with earliest deadlines first)
    for (int i = 0; i < num; i++) {
        order[i] = tasks[i].task_id;
    }
    return 1; // Schedulable
}

void check_rm(Task tasks[], int num) {
    for (int i = 0; i < num; i++) {
        if (tasks[i].Di != tasks[i].Ti) {
            printf("Di != Ti, RM not applicable.\n");
            return;
        }
    }

    double U = utilization_factor(tasks, num);
    printf("Utilization factor U = %.2f\n", U);

    double LL_bound = liu_layland_bound(num);
    printf("Liu-Layland Bound = %.2f\n", LL_bound);

    if (U > 1) {
        printf("U > 1, task set is not schedulable under RM.\n");
        return;
    }

    if (U <= LL_bound) {
        printf("Task set is Schedulable under Liu-Layland bound.\n");
    } else {
        printf("Utilization exceeds Liu-Layland bound.\n");
    }

    if (hyperbolic_bound(tasks, num)) {
        printf("Task set is Schedulable under hyperbolic bound.\n");
    } else {
        printf("Hyperbolic bound not satisfied.\n");
    }

    // Sort tasks by Time Period (Ti) for RM
    sort_by_period(tasks, num);

    int order[MAX_TASKS];
    if (response_time_analysis(tasks, num, order, 1)) {
        printf("Task set is Schedulable under Response Time Analysis (RTA).\n");
        printf("Task execution order (RM): ");
        for (int i = 0; i < num; i++) {
            printf("T%d ", order[i]);
        }
        printf("\n");
    } else {
        printf("Task set is Not schedulable under RM using RTA.\n");
    }
}

void check_dm(Task tasks[], int num) {
    for (int i = 0; i < num; i++) {
        if (tasks[i].Di > tasks[i].Ti) {
            printf("Di > Ti, DM not applicable.\n");
            return;
        }
    }

    double U = utilization_factor(tasks, num);
    printf("Utilization factor U = %.2f\n", U);

    // Sort tasks by Relative Deadline (Di) for DM
    sort_by_deadline(tasks, num);

    int order[MAX_TASKS];
    if (U < 1 && response_time_analysis(tasks, num, order, 0)) {
        printf("Task set is Schedulable under DM with Response Time Analysis (RTA).\n");
        printf("Task execution order (DM): ");
        for (int i = 0; i < num; i++) {
            printf("T%d ", order[i]);
        }
        printf("\n");
    } else {
        printf("Task set is Not schedulable under DM.\n");
    }
}

void check_edf(Task tasks[], int num) {
    double U = utilization_factor(tasks, num);
    printf("Utilization factor U = %.2f\n", U);

    if (U > 1) {
        printf("U > 1, task set is not schedulable under EDF.\n");
        return;
    }

    int order[MAX_TASKS];
    if (U < 1 || processor_demand_analysis(tasks, num, order)) {
        printf("Task set is Schedulable under EDF with Processor Demand Analysis (PDA).\n");
        printf("Task execution order (EDF): ");
        for (int i = 0; i < num; i++) {
            printf("T%d ", order[i]);
        }
        printf("\n");
    } else {
        printf("Task set is Not schedulable under EDF.\n");
    }
}

int main() {
    int num, algo;
    Task tasks[MAX_TASKS];

    printf("Enter number of tasks: ");
    scanf("%d", &num);

    input_tasks(tasks, num);

    printf("Select scheduling algorithm: (1) RM (2) DM (3) EDF: ");
    scanf("%d", &algo);

    switch (algo) {
        case 1:
            check_rm(tasks, num);
            break;
        case 2:
            check_dm(tasks, num);
            break;
        case 3:
            check_edf(tasks, num);
            break;
        default:
            printf("Invalid algorithm selection.\n");
            break;
    }

    return 0;
}
