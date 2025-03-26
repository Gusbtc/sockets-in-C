#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(){
    const int PORT = 9999;
    char enviar[1024];
    char buffer[1024];
    int s;
    struct sockaddr_in clientaddr;
    
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0){
        printf("[-] Error when creating socket\n");
        exit(1);
    }

    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(PORT);
    clientaddr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*) &clientaddr, sizeof(clientaddr));
    listen(s, 5);
    printf("[+] Listening on port %d...\n",PORT);

    int addrlen = sizeof(clientaddr);
    int connection = accept(s, (struct sockaddr*) &clientaddr,(socklen_t*)&addrlen);

    printf("[+] Connected. Waiting for data...\n");

    while (1){

        memset(buffer, 0, sizeof(buffer)); // Resetar o que esta salvo no buffer
        int received = recv(connection, buffer, sizeof(buffer) -1, 0); // -1 para deixar espaco para o null byte
        if (received <= 0){
            printf("[-] Client disconnected.\n");
            break;
        }
        printf("Client -> %s", buffer);

        printf("Server -> ");
        fgets(enviar, 1024, stdin);

        if (strcmp(enviar, "/exit\n") == 0){ // Receber "comandos"
            break;
        }

        send(connection, enviar, strlen(enviar), 0);
        
    }

    printf("[-] Closing socket...\n");
    close(connection);
    close(s);

    return 0;
}
