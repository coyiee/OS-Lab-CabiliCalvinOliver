# Lab 2 — CPU Scheduling (C)

This lab implements three CPU scheduling algorithms and prints a table, averages, and a text Gantt chart.

- Algorithms: **FCFS**, **SJF (non-preemptive)**, **Round Robin**
- Formulas:  
  - **TAT** = CT − AT  
  - **WT**  = TAT − BT

---

## Build & Run (WSL / Ubuntu)

```bash
cd Lab2_Scheduling
gcc -std=c11 cpu_scheduling.c -o cpu_scheduling
./cpu_scheduling
