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

int clients[MAX_CLIENTS];
pthread_mutex_t look = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const char* message, int client_id){
    pthread_mutex_lock(&look);
    for(int i=0; i<MAX_CLIENTS; i++){
        if(clients[i] != 0 && clients[i] != client_id){
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&look);
}

void* handle_client(void *arg){
    int client_id = *(int*)arg;
    char buffer[MAX_MSG]; 
    ssize_t len;

    while(len = recv(client_id, buffer, sizeof(buffer), 0) > 0){
        printf("Received message from client %d: %s\n", client_id, buffer);
        broadcast(buffer, client_id);
    }

    close(client_id);
    pthread_mutex_lock(&look);
    for(int i=0; i<MAX_CLIENTS; i++){
        if(clients[i] == client_id){
            clients[i] = 0;
        }
    }
    pthread_mutex_unlock(&look);

    return NULL;
}

int main(int argc, char const *argv[])
{
    int server_id, client_id;
    struct sockaddr_in server_addr, client_addr;
    pthread_t(client_thread);
    socklen_t socklen = sizeof(client_addr);

    server_id = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_id, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_id, MAX_CLIENTS);
    printf("Server started on port %d\n", PORT);

    while(client_id = accept(server_id, (struct sockaddr*)&client_addr, &socklen))
    {
        printf("Client connected: %d\n", client_id);
        

        pthread_mutex_lock(&look);
        for(int i=0; i<MAX_CLIENTS; i++){
            if(clients[i] == 0){
                clients[i] = client_id;
                break;
            }
        }
        pthread_mutex_unlock(&look);

        pthread_create(&client_thread, NULL, handle_client, &client_id );
        pthread_detach(client_thread);
    }
    close(server_id);
    printf("Server stopped\n");

    
    return 0;
}
