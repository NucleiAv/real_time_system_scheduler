# Fish
This is a real time simulating project about the swarm behavior. 

**Problem Statement** - Simulate a school of fishes exhibiting a swarm behavior, where a leader moves pseudo randomly and the other fishes follow the neighbors (also with some random factor). Fishes can also recognize the presence of a predator so they can escape from it. The predator can be introduced and animated by the user through the mouse pointer.

We have used the Rate Monotonic (RM) Scheduling Algo for Multi-threads. The Kernel is set to work on FIFO to minimize its interferance in our coustomization in the code.
Thus there are 2 tasks majorly - periodic (fish) and aperiodic (predator). Since the priority and time period is same for all task, we used RM with FIFO - the thread coming first is executed first in RM. 

## Diagrams and Flowcharts

![image](https://github.com/user-attachments/assets/67c322e3-3199-462d-872a-6bcb5eb7f4ac)
![image](https://github.com/user-attachments/assets/a5bf911f-4b56-424e-abd3-ea47a88d1f98)
![image](https://github.com/user-attachments/assets/4bb743a2-c688-4fd1-b86c-b9a84588e63b)
![image](https://github.com/user-attachments/assets/eb1d709c-5f46-4723-b096-3196f1c5db35)
![image](https://github.com/user-attachments/assets/d2eb9c13-cd6e-4e04-abdb-bd12554069d9)


## Video Simulation

**Click on the below image to get redirected to the video simulation.**


[![Video](https://github.com/user-attachments/assets/90d78094-6b52-4d29-b094-02afaf549cd7)](https://youtu.be/fBIX9BvheSw)


