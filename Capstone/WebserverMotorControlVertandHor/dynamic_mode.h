#ifndef DYNAMIC_MODE_H
#define DYNAMIC_MODE_H

// Constants
#define NUM_LOCATIONS 12
#define HISTORY_SIZE 3
#define OPTIONS_PER_LOCATION 4

// Global variables
extern int shot_history[HISTORY_SIZE];
extern int history_index;
extern int optimal_shots[NUM_LOCATIONS][OPTIONS_PER_LOCATION];

// Function declarations
int is_in_history(int shot);
void shift_selected_to_back(int location, int selected);
int select_next_shot();
void add_to_history(int shot);
void run_dynamic_mode(int rounds);

#endif // DYNAMIC_MODE_H
