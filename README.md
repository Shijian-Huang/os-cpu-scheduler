# os-cpu-scheduler

A C-based simulator for evaluating classic CPU scheduling algorithms. 

## 📋 Description
Simulates and compares six CPU scheduling algorithms:
- FCFS (First-Come, First-Served)
- SJF (Shortest Job First) [Non-preemptive]
- SRT (Shortest Remaining Time) [Preemptive]
- RR (Round Robin) [Preemptive]
- HPF (Highest Priority First) [Preemptive and Non-preemptive]

Each algorithm processes a randomly generated list of tasks and outputs:
- Gantt-style scheduling logs
- Response time, waiting time, turnaround time
- Average performance metrics for 5 rounds of simulation

## 📁 Project Structure
```
os-cpu-scheduler/
├── src/                 # All .c source files
├── include/             # All .h header files
├── build/               # Compiled object files (generated)
├── Makefile             # Build automation
├── scheduler            # Compiled output binary
└── output          # Output from simulation runs
```

## 🧪 How to Compile & Run
```bash
make              # Compile the project
./scheduler       # Run the simulator
```

The output will be saved to `output/report.txt`.

## 🧼 Clean up build artifacts
```bash
make clean
```

## 📌 Dependencies
Standard C library only. No external dependencies.