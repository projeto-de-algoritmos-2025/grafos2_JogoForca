#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 20
#define MAX_MSG 1000
#define PORT 8080

int clients[MAX_CLIENTS];
pthread_mutex_t look = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const char* message){

}

void hndle_client(void *arg){
    int client_id = *(int*)arg;
}

int main(int argc, char const *argv[])
{
    int server_id, client_id;
    struct sockaddr_in server_addr, client_addr;
    pthread_t(client_threads);

    server_id = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_id, &server_addr, sizeof(server_addr));
    listen(server_id, MAX_CLIENTS);
    printf("Server started on port %d\n", PORT);

    while(client_id = accept(server_id, &client_addr, sizeof(client_addr)))
    {
        printf("Client connected: %d\n", client_id);
        
        for(int i=0; i<MAX_CLIENTS; i++){
            if(clients[i] == 0){
                clients[i] = client_id;
                break;
            }
        }

        pthread_create(&client_addr, NULL, hndle_client, &client_id );
        pthread_detach(client_threads);
    }
    close(server_id);
    printf("Server stopped\n");

    
    return 0;
}
