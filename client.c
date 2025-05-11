#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_CLIENTS 20
#define MAX_MSG 1000
#define PORT 8080

    void* handleMessages(void* arg){
        int client_id = *(int*)arg;
        char buffer[MAX_MSG]; 
        ssize_t len;

        while(len = recv(client_id, buffer, sizeof(buffer), 0) > 0){
            buffer[len] = '\0'; 
            printf("Received message from client %d: %s\n", client_id, buffer);
        }

        close(client_id);

        return NULL;
    }

int main(){
    int client_id;
    struct sockaddr_in sockaddr;
    pthread_t lister;
    char msg[MAX_MSG];

    client_id = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(client_id, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    printf("Connected to server\n");

    pthread_create(&lister, NULL, handleMessages, &client_id);

    while(fgets(msg, MAX_MSG, stdin)){
        send(client_id, msg, strlen(msg), 0);
    }

    close(client_id);

    return 0;
}