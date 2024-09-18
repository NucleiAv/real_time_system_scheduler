# Real-Time Scheduling Analysis

Made by Anmol Vats. Don't forget to ⭐

## Overview

This project provides a C program for analyzing and predicting the schedulability of real-time periodic tasks using three common scheduling algorithms:
- Rate-Monotonic (RM)
- Deadline-Monotonic (DM)
- Earliest Deadline First (EDF)

## Algorithms

### Rate-Monotonic Scheduling (RM)

**Rate-Monotonic Scheduling** assigns priorities based on the task periods: tasks with shorter periods have higher priorities.

- **Necessary Condition**:
  - All tasks must have **Deadline = Period** (i.e., \( $D_i = T_i$ \) for all tasks).
  - **Utilization Factor** \( U \) should be :
  
    $$U = \sum_{i=1}^{n} \frac{C_i}{T_i}$$
    
  - If \( U > 1 \), the task set is not schedulable.

- **Sufficient Conditions**:
  1. **Liu-Layland Bound**: The Liu-Layland Bound for \( n \) tasks is:

  $U_{LL} = n \left(2^{\frac{1}{n}} - 1\right)$
  The task set is schedulable under RM if \( $U \leq U_{LL}$ \).

  2. **Hyperbolic Bound**:
  
     $\prod_{i=1}^{n} \left(\frac{C_i}{T_i} + 1\right) \leq 2$
     
     If the hyperbolic bound condition is satisfied, the task set is schedulable.

  - **Exact Condition**:
    - **Response Time Analysis (RTA)**:
    
      The response time \( $R_i$ \) for task \( $i$ \) should not exceed its deadline \( $D_i$ \). The response time analysis involves iterative calculation:

      $R_i = C_i + \sum_{j=1}^{i-1} \left\lceil \frac{R_i}{T_j} \right\rceil C_j$
      
      The task set is schedulable if \( $R_i \leq D_i$ \) for all tasks.

### Deadline-Monotonic Scheduling (DM)

**Deadline-Monotonic Scheduling** prioritizes tasks based on their deadlines: tasks with earlier deadlines have higher priorities.

- **Necessary Condition**:
  - All tasks must have **Deadline ≤ Period** (i.e., \( $D_i \leq T_i$ \) for all tasks).
  - **Utilization Factor** \( $U$ \) should be \( $\leq 1$ \):

    $U = \sum_{i=1}^{n} \frac{C_i}{T_i}$

- **Exact Condition**:
  - **Response Time Analysis (RTA)**:

    The response time \( $R_i$ \) for task \( $i$ \) should not exceed its deadline \( $D_i$ \). The response time analysis involves iterative calculation:

    $R_i = C_i + \sum_{j=1}^{i-1} \left\lceil \frac{R_i}{T_j} \right\rceil C_j$

    The task set is schedulable if \( $R_i \leq D_i$ \) for all tasks.

### Earliest Deadline First (EDF)

**Earliest Deadline First** dynamically prioritizes tasks based on the closest deadline: tasks with earlier deadlines are given higher priority.

- **Necessary Condition**:
  - **Utilization Factor** \( $U$ \) should be \( $\leq 1$ \):

    $U = \sum_{i=1}^{n} \frac{C_i}{T_i}$
  
  - If \( $U > 1$ \), the task set is not schedulable.

- **Sufficient Condition**:
  - **Utilization Factor** \( $U$ \) should be \( $\leq 1$ \):

    $U < 1$

- **Exact Condition**:
  - **Processor Demand Criteria (PDC)**:

    In Earliest Deadline First (EDF) scheduling, the **processor demand criterion** ensures that all tasks can meet their deadlines. It checks if the total CPU utilization required by all tasks within any time interval \($[t_1, t_2]$ \) is less than or equal to the length of that interval.

The processor demand criterion is given by the following formula:

$\text{Processor Demand} = \sum_{\tau_i \in \text{tasks}} \left( \left\lfloor \frac{t_2 - D_i + T_i}{T_i} \right\rfloor + 1 \right) C_i $

Where:
- $\tau_i$ refers to the i-th task.
- $C_i$  is the execution time of task $\tau_i$.
- $T_i$  is the period of task $\tau_i$.
- $D_i$  is the relative deadline of task $\tau_i$.
- $[t_1, t_2]$  is the interval over which the processor demand is calculated.
- $\left\lfloor x \right\rfloor$ denotes the floor function, which returns the greatest integer less than or equal to $x$.

The EDF scheduling algorithm can schedule tasks feasibly as long as the processor demand over any interval is no more than the length of the interval:

$\text{Processor Demand} \leq (t_2 - t_1)$

If this condition holds for all intervals, the task set can be successfully scheduled using EDF.
## Parameters

- **Computation Time ($C_i$)**: Time required by a task to complete its execution.
- **Period ($T_i$)**: Time interval between successive releases of the task.
- **Deadline ($D_i$)**: Time by which the task must be completed.
- **Arrival Time ($a_i$)**: Time when a task enters.

## Files

- `RTS_scheduler.c`: Main implementation of the scheduling analysis.
- `README.md`: This documentation file.

## Usage

1. Clone the repository:

    ```bash
    git clone https://github.com/NucleiAv/real_time_system_scheduler
    ```

2. Compile the program:

    ```bash
    gcc RTS_scheduler.c -o RTS_scheduler -lm
    ```

3. Run the program:

    ```bash
    ./RTS_scheduler
    ```

4. Follow the on-screen prompts to input task parameters and select a scheduling algorithm.

## Example

```
Enter number of tasks: 3
Enter Computation time (Ci), Relative deadline (Di), Time period (Ti) for Task 1: 2 10 10
Enter Computation time (Ci), Relative deadline (Di), Time period (Ti) for Task 2: 1 20 20
Enter Computation time (Ci), Relative deadline (Di), Time period (Ti) for Task 3: 1 30 30
Select scheduling algorithm: (1) RM (2) DM (3) EDF: 1
```

## Contributing

Feel free to fork the repository and submit pull requests. For any issues or feature requests, please use the [issue tracker](https://github.com/yourusername/RealTimeSchedulingAnalysis/issues).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
