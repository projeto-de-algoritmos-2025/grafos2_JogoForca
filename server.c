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
#include <limits.h>
#include <stdbool.h>

#define MAX_CLIENTS 20
#define MAX_MSG 1000
#define PORT 8080

int clients[MAX_CLIENTS];
pthread_mutex_t look = PTHREAD_MUTEX_INITIALIZER;

// ----- Grafo e Dijkstra -----

typedef struct Edge {
    int to;
    int weight;
    struct Edge* next;
} Edge;

Edge* graph_adj[MAX_CLIENTS];
int num_nodes = MAX_CLIENTS;

// Inicializa o grafo
void init_graph() {
    for(int i = 0; i < num_nodes; i++) {
        graph_adj[i] = NULL;
    }
}

// Adiciona aresta (direcionada) de u para v
void add_edge(int u, int v, int w) {
    Edge* e = malloc(sizeof(Edge));
    e->to = v;
    e->weight = w;
    e->next = graph_adj[u];
    graph_adj[u] = e;
}

// Encontra índice de nó a partir de um client_fd
int get_node_index(int client_fd) {
    for(int i = 0; i < num_nodes; i++) {
        if(clients[i] == client_fd) return i;
    }
    return -1;
}

// Implementação de Dijkstra
void dijkstra(int src, int dist[], int prev[]) {
    bool visited[MAX_CLIENTS] = {0};
    for(int i = 0; i < num_nodes; i++){
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[src] = 0;

    for(int iter = 0; iter < num_nodes; iter++) {
        int u = -1, best = INT_MAX;
        for(int i = 0; i < num_nodes; i++) {
            if(!visited[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if(u < 0) break;
        visited[u] = true;

        for(Edge* e = graph_adj[u]; e; e = e->next) {
            int v = e->to, w = e->weight;
            if(!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }
}

// Reconstrói o caminho de src até dest usando prev[]
int build_path(int dest, int prev[], int path[]) {
    int count = 0;
    for(int at = dest; at != -1; at = prev[at]) {
        path[count++] = at;
    }
    for(int i = 0; i < count/2; i++) {
        int tmp = path[i];
        path[i] = path[count-1-i];
        path[count-1-i] = tmp;
    }
    return count;
}

// ----- Funções de Chat -----

void broadcast(char* message, int sender_fd) {
    pthread_mutex_lock(&look);
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients[i] != 0 && clients[i] != sender_fd) {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&look);
}

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    char buffer[MAX_MSG];
    ssize_t len;

    while((len = recv(client_fd, buffer, sizeof(buffer)-1, 0)) > 0) {
        buffer[len] = '\0';
        printf("Received message from client %d: %s", client_fd, buffer);

        if(strncmp(buffer, "/connect ", 9) == 0) {
            int dest_fd, weight;
            if(sscanf(buffer + 9, "%d %d", &dest_fd, &weight) == 2) {
                pthread_mutex_lock(&look);
                int u = get_node_index(client_fd);
                int v = get_node_index(dest_fd);
                if(u >= 0 && v >= 0) {
                    add_edge(u, v, weight);
                    // Para grafo não-direcionado: add_edge(v, u, weight);
                    send(client_fd, "[OK] Connected\n", 15, 0);
                } else {
                    send(client_fd, "[ERROR] Invalid IDs\n", 21, 0);
                }
                pthread_mutex_unlock(&look);
            }
        }
        else if(strncmp(buffer, "/path ", 6) == 0) {
            int dest_fd;
            if(sscanf(buffer + 6, "%d", &dest_fd) == 1) {
                int u = get_node_index(client_fd);
                int v = get_node_index(dest_fd);
                if(u >= 0 && v >= 0) {
                    int dist[MAX_CLIENTS], prev[MAX_CLIENTS], path[MAX_CLIENTS];
                    dijkstra(u, dist, prev);
                    int cnt = build_path(v, prev, path);
                    char out[2*MAX_MSG] = {0};
                    char tmp[64];
                    snprintf(out, sizeof(out), "[PATH] cost=%d: ", dist[v]);
                    for(int i = 0; i < cnt; i++) {
                        snprintf(tmp, sizeof(tmp), "%d ", path[i]);
                        strncat(out, tmp, sizeof(out) - strlen(out) - 1);
                    }
                    strncat(out, "\n", sizeof(out) - strlen(out) - 1);
                    send(client_fd, out, strlen(out), 0);
                }
            }
        }
        else if(strncmp(buffer, "/msg ", 5) == 0) {
            int dest_fd;
            char *msg_text = buffer + 5;
            if(sscanf(msg_text, "%d %[^\n]", &dest_fd, msg_text) == 2) {
                pthread_mutex_lock(&look);
                int u = get_node_index(client_fd);
                int v = get_node_index(dest_fd);
                if(u >= 0 && v >= 0) {
                    int dist[MAX_CLIENTS], prev[MAX_CLIENTS], path[MAX_CLIENTS];
                    dijkstra(u, dist, prev);
                    int cnt = build_path(v, prev, path);
                    if(cnt >= 2) {
                        int next_fd = clients[path[1]];
                        char fwd[MAX_MSG];
                        snprintf(fwd, sizeof(fwd), "[%d->%d] %s\n", client_fd, dest_fd, msg_text);
                        send(next_fd, fwd, strlen(fwd), 0);
                    }
                }
                pthread_mutex_unlock(&look);
            }
        }
        else {
            broadcast(buffer, client_fd);
        }
    }

    close(client_fd);
    pthread_mutex_lock(&look);
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients[i] == client_fd) {
            clients[i] = 0;
        }
    }
    pthread_mutex_unlock(&look);
    return NULL;
}

int main() {
    int server_id, client_id;
    struct sockaddr_in server_addr, client_addr;
    pthread_t client_thread;
    socklen_t socklen = sizeof(client_addr);

    // Inicializa grafo e lista de clientes
    init_graph();
    memset(clients, 0, sizeof(clients));

    server_id = socket(AF_INET, SOCK_STREAM, 0);
    if(server_id < 0) { perror("Socket failed"); exit(EXIT_FAILURE); }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_id, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed"); exit(EXIT_FAILURE);
    }
    if(listen(server_id, 5) < 0) {
        perror("Listen failed"); exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    while((client_id = accept(server_id, (struct sockaddr*)&client_addr, &socklen)) >= 0) {
        printf("Client connected: %d\n", client_id);
        pthread_mutex_lock(&look);
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if(clients[i] == 0) {
                clients[i] = client_id;
                break;
            }
        }
        pthread_mutex_unlock(&look);
        pthread_create(&client_thread, NULL, handle_client, &client_id);
        pthread_detach(client_thread);
    }

    close(server_id);
    printf("Server stopped\n");
    return 0;
}
