#include "forca.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

char** load_words(const char* filename, int* words_count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao ler arquivo %s\n", filename);
        return NULL;
    }

    char **words = malloc(MAX_WORDS * sizeof(char*));
    if (words == NULL) {
        printf("Erro ao armazenar dados na memória.\n");
        fclose(file);
        return NULL;
    }

    char buffer[MAX_WORD_SIZE];
    while (fgets(buffer, MAX_WORD_SIZE, file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 0) continue;

        words[*words_count] = malloc(strlen(buffer) + 1);
        strcpy(words[*words_count], buffer);
        (*words_count)++;
    }
    fclose(file);
    return words;
}

char *random_word(const char** words, int count) {
    srand(time(NULL));
    return (char*)words[rand() % count];
}

int play_game(const char* word, int wins) {
    int len = strlen(word);
    char *display = malloc(len + 1);
    for (int i = 0; i < len; i++)
        display[i] = '_';
    display[len] = '\0';

    int attempts = 0, corrects = 0;
    char wrong_letters[27] = "";

    WINDOW *game = newwin(20, 70, 2, 0);
    keypad(game, TRUE);

    while (attempts < MAX_ATTEMPTS && corrects < len) {
        draw_game(game, display, attempts, wrong_letters, wins);
        mvwprintw(game, 18, 2, "Digite uma letra: ");
        wrefresh(game);

        char guess = toupper(wgetch(game));
        if (guess < 'A' || guess > 'Z') continue;

        if (strchr(display, guess) || strchr(wrong_letters, guess))
            continue;

        int hit = 0;
        for (int i = 0; i < len; i++) {
            if (toupper(word[i]) == guess && display[i] == '_') {
                display[i] = guess;
                hit = 1;
                corrects++;
            }
        }

        if (!hit) {
            wrong_letters[strlen(wrong_letters)] = guess;
            attempts++;
        } else {
            beep();
        }
    }

    int result = 0;
    if (corrects == len) {
        werase(game);
        box(game, 0, 0);
        mvwprintw(game, 9, 20, "Parabéns! Você venceu!");
        beep(); beep();
        wrefresh(game);
        result = 1;
        napms(2000);
    } else {
        mvwprintw(game, 9, 20, "Você perdeu!");
        mvwprintw(game, 16, 2, "(R) Reiniciar e jogar novamente.");
        mvwprintw(game, 17, 2, "(S) Sair do jogo");
        wrefresh(game);

        int ch;
        do {
            ch = toupper(wgetch(game));
        } while (ch != 'R' && ch != 'S');

        result = (ch == 'R') ? 0 : -1;
    }

    delwin(game);
    free(display);
    return result;
}

void draw_game(WINDOW *game, const char *display, int attempts, const char* wrong_letters, int wins) {
    werase(game);
    box(game, 0, 0);

    mvwprintw(game, 1, 2, "Placar: %d", wins);
    mvwprintw(game, 2, 2, "Palavra: %s", display);
    mvwprintw(game, 3, 2, "Tentativas restantes: %d", MAX_ATTEMPTS - attempts);

    wattron(game, COLOR_PAIR(2));
    mvwprintw(game, 4, 2, "Letras erradas: %s", wrong_letters);
    wattroff(game, COLOR_PAIR(2));

    draw_hangman(game, attempts);
    wrefresh(game);
}

void draw_hangman(WINDOW *game, int attempts) {
    wattron(game, COLOR_PAIR(3));
    mvwprintw(game, 3, 50, "+----+");
    mvwprintw(game, 4, 50, "|    |");
    for (int i = 0; i < 4; i++)
        mvwprintw(game, 5 + i, 50, "     |");
    mvwprintw(game, 9, 50, "=======");
    wattroff(game, COLOR_PAIR(3));

    wattron(game, COLOR_PAIR(4));
    if (attempts > 0) mvwprintw(game, 5, 50, "O");
    if (attempts > 1) mvwprintw(game, 6, 50, "|");
    if (attempts > 2) mvwprintw(game, 6, 49, "/");
    if (attempts > 3) mvwprintw(game, 6, 51, "\\");
    if (attempts > 4) mvwprintw(game, 7, 49, "/");
    if (attempts > 5) mvwprintw(game, 7, 51, "\\");
    wattroff(game, COLOR_PAIR(4));
}
