#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <ncurses.h>

#define MAX_WORDS 300
#define MAX_WORD_SIZE 25
#define MAX_ATTEMPTS 6

char **load_words(const char* filename, int *word_count);
char *get_random_word(char **words, int count);
int play_game(const char *word, int wins);

int main(){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    int words_count = 0;
    char **words = (char **)malloc(10 * sizeof(char *));

    if(words == NULL || words_count < 1){
        printf("Erro ao ler o arquivo palavras.txt\n");
        return EXIT_FAILURE;
    }

    int exitGame = 0;
    int wins = 0;
    while (!exitGame)
    {
        char word = random_word(words, words_count);
        int result = play_game(word, wins);
        if(result == 1){
            wins++;

        } else if(result == 0){
            wins = 0;
        } else if(result == -1) {
            exitGame = 1;
        }
    }
    

    return 0;
}


char **load_words(const char* filename, int *word_count){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao ler o arquivo palavras.txt\n");
        return NULL;
    }


    char **words = (char **)malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) {
        printf("Erro ao armazenar dados na memória\n");
        return NULL;
    }

    char buffer[MAX_WORD_SIZE];
    
    while (fgets(buffer, MAX_WORDS, file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        if (words == NULL) {
            continue;
            }


            words[*word_count] = malloc(sizeof(char) * (strlen(buffer) + 1));
            strcpy(words[*word_count], buffer);
            (word_count)++;
            return words;
        
        
    }

    
    return words;
}

char *get_random_word(char **words, int count){
    int number = rand() % count;
}

int play_game(const char *word, int wins){
    int len = strlen(word);
    char *display = malloc(len + 1);
    for(int i=0; i < len; i++){
        display[i] = '_';
    }

    int attempts = 0;
    int correct = 0;

    WINDOW *win = newwin(20, 40, 0, 0);

    while(attempts < MAX_ATTEMPTS && correct < len){

    }
}