#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <ncurses.h>

#define MAX_WORDS 300
#define MAX_WORD_SIZE 25

char **load_words(const char* filename, int *word_count);

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
    while (!exitGame)
    {
        
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