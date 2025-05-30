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

// Optimal next shot options per location (ordered by priority)
int optimal_shots[NUM_LOCATIONS][OPTIONS_PER_LOCATION] = {
    {11, 7, 5, 3},  // 0: follow up with front-right, mid-right, center-mid, or deep-right
    {10, 6, 4, 2},  // 1: front-center-right, mid-right, mid-left, deep-center-right
    {8, 5, 1, 3},   // 2: sharp drop left, center-mid, re-attack back-left, or same side deep
    {8, 4, 6, 1},   // 3: front-left, mid-left, mid-right, or cross to 1
    {3, 9, 11, 6},  // 4: deep-right, front-center-left, front-right, mid-right
    {0, 10, 8, 7},  // 5: deep-left, front-center-right, front-left, mid-far-right
    {1, 8, 9, 11},  // 6: deep-left, front-left, front-center-left, front-right
    {0, 4, 9, 2},   // 7: deep-left, mid-left, front-center-left, back-center-right
    {2, 6, 5, 10},  // 8: deep-right, mid-right, mid-center, front-center-right
    {3, 7, 1, 8},   // 9: deep-right, mid-right, deep-left, front-left
    {0, 5, 4, 9},   //10: deep-left, mid-center, mid-left, front-center-left
    {1, 6, 3, 8}    //11: deep-center-left, mid-right, deep-right, front-left
};

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
        if (optimal_shots[location][i] != selected) {
            new_order[idx] = optimal_shots[location][i];
            idx++;
        }
    }

    // Place selected at the end
    new_order[idx] = selected;

    // Copy back
    for (int i = 0; i < OPTIONS_PER_LOCATION; i++) {
        optimal_shots[location][i] = new_order[i];
    }
}

int select_next_shot() {
    // int last_shot = -1;

    int last_index = (history_index + HISTORY_SIZE - 1) % HISTORY_SIZE; 
    int last_shot = shot_history[last_index];


    // If no valid last shot, pick a random location
    if (last_shot == -1) {
        // printf("No last shot found, picking random location.\n");
        return rand() % NUM_LOCATIONS;
    }

    // Check each option in priority order, return the first not in history
    for (int i = 0; i < OPTIONS_PER_LOCATION; i++) {
        int candidate = optimal_shots[last_shot][i];
        if (!is_in_history(candidate)) {
            shift_selected_to_back(last_shot, candidate);
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


void run_dynamic_mode(int rounds) {
    for (int i = 0; i < rounds; i++) {
        int next_shot = select_next_shot();
        add_to_history(next_shot);

        /*Serial.println("Round %d: Shot to location %d | History: [", i + 1, next_shot);
        for (int j = 0; j < HISTORY_SIZE; j++) {
            Serial.println("%d", shot_history[j]);
            if (j < HISTORY_SIZE - 1) Serial.println(", ");
        }
        Serial.println("]\n");*/

        Serial.print("Round ");
        Serial.print(i + 1);
        Serial.print(": Shot to location ");
        Serial.print(next_shot);
        Serial.print(" | History: [");

        for (int j = 0; j < HISTORY_SIZE; j++) {
            Serial.print(shot_history[j]);
            if (j < HISTORY_SIZE - 1) Serial.print(", ");
        }

        Serial.println("]");

    }
}

int main() {
    srand(time(NULL)); // Seed RNG
    run_dynamic_mode(12);
    return 0;
}


