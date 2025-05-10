#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 20
#define MAX_MSG 1000
#define PORT 8080

int clients[MAX_CLIENTS];

int main(int argc, char const *argv[])
{
    int server_id, client_id;
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_id, &server_addr, sizeof(server_addr));
    listen(server_id, MAX_CLIENTS);
    printf("Server started on port %d\n", PORT);

    while(client_id = accept(server_id, &client_addr, sizeof(client_addr)))
    {
        printf("Client connected: %d\n", client_id);
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i] == 0)
            {
                clients[i] = client_id;
                break;
            }
        }

    }

    
    return 0;
}
