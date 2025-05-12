#include "forca.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    int words_count = 0;
    char **words = load_words("palavras.txt", &words_count);
    if (!words) return EXIT_FAILURE;

    int wins = 0, exitGame = 0;
    while (!exitGame) {
        char* word = random_word((const char**)words, words_count);
        int result = play_game(word, wins);
        if (result == 1) wins++;
        else if (result == 0) wins = 0;
        else exitGame = 1;
    }

    endwin();
    return EXIT_SUCCESS;
}
