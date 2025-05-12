#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <ncurses.h>

#define MAX_WORDS 300
#define MAX_WORD_SIZE 25
#define MAX_ATTEMPTS 6

char** load_words(const char* filename, int *word_count);
char* random_word(const char **words, int count);
int play_game(const char *word, int wins);
void draw_game(WINDOW *game ,const char *display, int attempts, char *wrong_letters, int wins);

int main(){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    int words_count = 0;
    char **words = load_words("palavras.txt", &words_count);
    if (words == NULL || words_count < 1) {
        return EXIT_FAILURE;
    }

    int exitGame = 0;
    int wins = 0;
    while (!exitGame) {
        char* word = random_word((const char**)words, words_count);
        int result = play_game(word, wins);

        if (result == 1) {
            wins++;
        } else if (result == 0) {
            wins = 0;
        } else if (result == -1) {
            exitGame = 1;
        }
    }

    endwin();

    return EXIT_SUCCESS;
}


char **load_words(const char* filename, int* word_count){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao ler o arquivo palavras.txt\n");
        return NULL;
    }

    *word_count = 0;

    char** words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) {
        printf("Erro ao armazenar dados na memória\n");
        return NULL;
    }

    char buffer[MAX_WORD_SIZE];
    
    while (fgets(buffer, MAX_WORDS, file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        if (strlen(buffer) == 0) {
            continue;
            }


            words[*word_count] = malloc(strlen(buffer) + 1);
            strcpy(words[*word_count], buffer);
            (*word_count)++;        
        
    }
    fclose(file);

    
    return words;
}

char *random_word(const char** words, int count){
    return (char*)words[rand() % count];
}

int play_game(const char *word, int wins){
    int len = strlen(word);
    char *display = malloc(len + 1);
    for(int i=0; i < len; i++){
        display[i] = '_';
    }

    display[len] = '\0';
    int result = 0;

    int attempts = 0;
    int correct = 0;
    char wrong_letters[27] = "";

    WINDOW *game = newwin(20, 70, 2, 0);
    keypad(game, TRUE);

    while(attempts < MAX_ATTEMPTS && correct < len){
        draw_game(game, display, attempts, wrong_letters, wins);
    }
}

void draw_game(WINDOW *game ,const char *display, int attempts, char *wrong_letters, int wins){
    werase(game);
    box(game, 0, 0);
    mvwprintw(game, 1, 2, "Placar: %d", wins);
    mvwprintw(game, 3, 1, "Tentativas restantes: %d", MAX_ATTEMPTS - attempts);
    mvwprintw(game, 5, 1, "Letras erradas: %s", wrong_letters);
    wrefresh(game);

}