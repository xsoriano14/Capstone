#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dynamic_mode.h"
#include <Arduino.h>

// #define NUM_LOCATIONS 12
// #define HISTORY_SIZE 3
// #define OPTIONS_PER_LOCATION 4

// History queue (circular buffer)
int shot_history[HISTORY_SIZE] = {-1, -1, -1}; 
int history_index = 0;

/*
// Optimal next shot options per location (ordered by priority)
int optimal_shots_hard[NUM_LOCATIONS][OPTIONS_PER_LOCATION] = {
  {12, 8, 6, 4},  // 0: follow up with front-right, mid-right, center-mid, or deep-right  
  {11, 7, 5, 3},  // 1: front-center-right, mid-right, mid-left, deep-center-right  
  {9, 6, 2, 4},   // 2: sharp drop left, center-mid, re-attack back-left, or same side deep  
  {9, 5, 7, 2},   // 3: front-left, mid-left, mid-right, or cross to 1  
  {4, 10, 12, 7}, // 4: deep-right, front-center-left, front-right, mid-right  
  {1, 11, 9, 8},  // 5: deep-left, front-center-right, front-left, mid-far-right  
  {2, 9, 10, 12}, // 6: deep-left, front-left, front-center-left, front-right  
  {1, 5, 10, 3},  // 7: deep-left, mid-left, front-center-left, back-center-right  
  {3, 7, 6, 11},  // 8: deep-right, mid-right, mid-center, front-center-right  
  {4, 8, 2, 9},   // 9: deep-right, mid-right, deep-left, front-left  
  {1, 6, 5, 10},  //10: deep-left, mid-center, mid-left, front-center-left  
  {2, 7, 4, 9}    //11: deep-center-left, mid-right, deep-right, front-left 
};*/

int optimal_shots_hard[NUM_LOCATIONS * OPTIONS_PER_LOCATION] = {
    12, 8, 6, 4,   // 1
    11, 7, 5, 3,   // 2
    9, 6, 2, 4,    // 3
    9, 5, 7, 2,    // 4
    4, 10, 12, 7,  // 5
    1, 11, 9, 8,   // 6
    2, 9, 10, 12,  // 7
    1, 5, 10, 3,   // 8
    3, 7, 6, 11,   // 9
    4, 8, 2, 9,    // 10
    1, 6, 5, 10,   // 11
    2, 7, 4, 9     // 12
};

int optimal_shots_medium[NUM_LOCATIONS * OPTIONS_PER_LOCATION] = {
    6, 8, 4, 12,   // 1
    7, 5, 3, 11,   // 2
    6, 9, 2, 4,    // 3
    5, 9, 2, 7,    // 4
    10, 4, 7, 12,  // 5
    11, 1, 8, 9,   // 6
    9, 2, 12, 10,  // 7
    5, 1, 3, 10,   // 8
    7, 3, 11, 6,   // 9
    8, 4, 9, 2,    // 10
    6, 1, 10, 5,   // 11
    7, 2, 9, 4     // 12
};

int optimal_shots_easy[NUM_LOCATIONS * OPTIONS_PER_LOCATION] = {
    8, 12, 6, 4,   // 0
    7, 11, 5, 3,   // 1
    6, 9, 2, 4,    // 2
    5, 9, 2, 7,    // 3
    10, 4, 7, 12,  // 4
    1, 11, 8, 9,   // 5
    9, 2, 10, 12,  // 6
    5, 1, 10, 3,   // 7
    7, 3, 11, 6,   // 8
    8, 4, 9, 2,    // 9
    6, 1, 5, 10,   // 10
    7, 2, 4, 9     // 11
};

int* optimal_shots = optimal_shots_hard;

void set_mode(int mode) {
    switch (mode) {
        case 1:
            optimal_shots = optimal_shots_hard;
            break;
        case 2:
            optimal_shots = optimal_shots_medium;
            break;
        case 3:
            optimal_shots = optimal_shots_easy;
            break;
        default:
            optimal_shots = optimal_shots_hard;
            break;
    }
}


// Check if a shot is in the recent history
int is_in_history(int shot) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (shot_history[i] == shot) return 1;
    }
    return 0;
}

// Shift selected shot to the end and reorder the rest
void shift_selected_to_back(int location, int selected) {
    int new_order[OPTIONS_PER_LOCATION];
    int idx = 0;

    // First, add all others in original order (excluding the selected one)  
    for (int i = 0; i < OPTIONS_PER_LOCATION; i++) {
      int original = optimal_shots[(location-1) * OPTIONS_PER_LOCATION + i];
      if (original != selected) {
          new_order[idx++] = original;
      }
    }

    // Place selected at the end
    new_order[idx] = selected;

    // Copy back
    for (int i = 0; i < OPTIONS_PER_LOCATION; i++) {
      optimal_shots[(location-1) * OPTIONS_PER_LOCATION + i] = new_order[i];    }
}

int select_next_shot() {
    // int last_shot = -1;

    int last_index = (history_index + HISTORY_SIZE - 1) % HISTORY_SIZE; 
    int last_shot = shot_history[last_index];


    // If no valid last shot, pick a random location
    if (last_shot == -1) {
         Serial.println("No last shot found, picking random location.\n");
                  int temp = random(1, 13);

                  Serial.println("No last shot found, picking random location.\n");
                  Serial.println(temp);

        return temp;
    }

    // Check each option in priority order, return the first not in history
    for (int i = 0; i < OPTIONS_PER_LOCATION; i++) {
        int candidate = optimal_shots[(last_shot - 1) * OPTIONS_PER_LOCATION + i];
        if (!is_in_history(candidate)) {
            shift_selected_to_back(last_shot, candidate);


            for (int loc = 0; loc < NUM_LOCATIONS; loc++) {
    Serial.print("Location ");
    Serial.print(loc+1);
    Serial.print(": ");
    for (int i = 0; i < OPTIONS_PER_LOCATION; i++) {
      int value = optimal_shots[loc * OPTIONS_PER_LOCATION + i];
      Serial.print(value);
      Serial.print(" ");
    }
    Serial.println();
  } 



            return candidate;
        }
    }

  
    // Fallback: All options are in history, just use the highest priority one
    // int fallback = optimal_shots[last_shot][0];
    // shift_selected_to_back(last_shot, fallback);
    // return fallback;
}

void add_to_history(int shot) {
    shot_history[history_index] = shot;
    history_index = (history_index + 1) % HISTORY_SIZE;
}




int main() {
    srand(time(NULL)); // Seed RNG
    //run_dynamic_mode(12);
    return 0;
}


