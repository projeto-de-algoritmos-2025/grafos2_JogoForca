#include "hint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_winner(const char* mask) {
    return strchr(mask, '_') == NULL;
}

State* create_state(const char* mask, int wrongs, int cost, State* prev) {
    State* new_state = (State*)malloc(sizeof(State));
    if (new_state == NULL) return NULL;
    
    strncpy(new_state->mask, mask, 63);
    new_state->mask[63] = '\0'; // Garante terminação nula
    new_state->wrongs = wrongs;
    new_state->cost = cost;
    new_state->prev = prev;
    return new_state;
}

int letter_used(const char* mask, const char* wrong_letters, char c) {
    return (mask && strchr(mask, c)) || (wrong_letters && strchr(wrong_letters, c));
}

void free_states(State** queue, int count) {
    for (int i = 0; i < count; i++) {
        if (queue[i]) free(queue[i]);
    }
}

char get_best_hint(const char* word, const char* current_mask, const char* wrong_letters) {
    if (!word || !current_mask || !wrong_letters) return '?';
    
    State *queue[10000];
    int front = 0, back = 0;

    State *initial = create_state(current_mask, strlen(wrong_letters), 0, NULL);
    if (!initial) return '?';
    
    queue[back++] = initial;

    while (front < back && back < 10000) {
        State *current = queue[front++];
        if (!current) continue;

        if (is_winner(current->mask)) {
            State *hint_state = current;
            while (hint_state->prev && hint_state->prev->prev != NULL)
                hint_state = hint_state->prev;

            char next_guess = '?';
            for (int i = 0; word[i]; i++) {
                if (hint_state->mask[i] != current_mask[i] && current_mask[i] == '_') {
                    next_guess = hint_state->mask[i];
                    break;
                }
            }

            // Liberar a memória alocada
            free_states(queue, back);
            return next_guess;
        }

        for (char c = 'A'; c <= 'Z'; c++) {
            if (letter_used(current->mask, wrong_letters, c))
                continue;

            char next_mask[64];
            strncpy(next_mask, current->mask, 63);
            next_mask[63] = '\0';

            int hit = 0;
            for (int i = 0; word[i] && i < 63; i++) {
                if (toupper(word[i]) == c) {
                    next_mask[i] = c;
                    hit = 1;
                }
            }

            int next_wrongs = current->wrongs + (hit ? 0 : 1);
            State *next_state = create_state(next_mask, next_wrongs, current->cost + (hit ? 0 : 1), current);
            
            if (next_state && back < 9999) {
                queue[back++] = next_state;
            }
        }
    }

    // Liberar a memória alocada
    free_states(queue, back);
    return '?';
}