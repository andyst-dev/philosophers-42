# philosophers

A concurrent programming project developed as part of the 42 curriculum.
This project consists of simulating the dining philosophers problem using threads and mutexes, with careful handling of timing, shared resources, and synchronization constraints.

`philosophers` was designed to strengthen core C skills around multithreading, mutex-based synchronization, timing control, and race-condition prevention.

## Features
- Philosopher simulation with one thread per philosopher
- Fork access protected with mutexes
- State transitions for thinking, eating, sleeping, and death
- Command-line configuration for timing and meal limits

## Project structure
- `inc/philo.h` — main header file, structures, enums, and function prototypes
- `src/main.c` — argument validation and program entry point
- `src/init.c` — table setup, philosopher initialization, and thread startup
- `src/routine.c` — philosopher routine and state progression
- `src/forks.c` — fork acquisition logic
- `src/state.c` — state checks and updates
- `src/utils.c` — utility helpers for parsing, allocation, and timing
- `Makefile` — builds the `philo` executable

## Mandatory part
The mandatory part implements the dining philosophers simulation with threads and mutexes.

### Program
- `philo` — runs the simulation with the required timing arguments and optional meal limit

### Core behavior
- creates one thread per philosopher
- protects shared fork access with mutexes
- logs philosopher actions with timestamps
- stops the simulation when a philosopher dies
- optionally stops when every philosopher has eaten enough times
- avoids overlapping output messages through synchronized printing

### What happens at runtime
- the program parses the command-line arguments and initializes the table state
- each philosopher is created as a separate thread
- philosophers repeatedly try to take two forks, eat, sleep, and think
- each action is printed with a timestamp and philosopher id
- the simulation ends as soon as one philosopher dies, or when the optional meal target is reached for every philosopher

### Subject requirements to respect
- no global variables are allowed
- each philosopher must be represented by a thread
- fork access must be protected to avoid data races
- log messages must not overlap
- a death message must be printed quickly after the actual death event
- the program must avoid unexpected crashes and memory leaks

## Build
Build the project:

```bash
make
```

Clean object files:

```bash
make clean
```

Remove object files and executable:

```bash
make fclean
```

Rebuild everything:

```bash
make re
```

## Output
The project builds one executable:

```bash
philo
```

## Usage
Run the program with the required timing arguments:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Example:

```bash
./philo 5 800 200 200
```

## Learning outcomes
This project was an introduction to concurrent programming in C.
It helped build solid foundations in:
- POSIX threads
- mutex-based synchronization
- shared resource coordination
- timing-sensitive state management
- race-condition awareness
- simulation design
