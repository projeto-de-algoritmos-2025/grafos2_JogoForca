#ifndef HINT_H
#define HINT_H

#define ALPHABET_SIZE 26
#define INF 1000000

typedef struct State {
    char mask[64];
    int wrongs;
    int cost;
    struct State *prev;
} State;

char get_best_hint(const char* word, const char* current_mask, const char* wrong_letters);

#endif
