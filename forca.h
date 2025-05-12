#ifndef FORCA_H
#define FORCA_H

#include <ncurses.h>

#define MAX_WORDS 300
#define MAX_WORD_SIZE 64
#define MAX_ATTEMPTS 6

char **load_words(const char* filename, int* words_count);
char *random_word(const char** words, int count);
int play_game(const char* word, int wins);
void draw_game(WINDOW *game, const char *display, int attempts, const char* wrong_letters, int wins);
void draw_hangman(WINDOW *game, int attempts);

#endif
