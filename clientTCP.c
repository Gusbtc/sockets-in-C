#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(){
    const char ip[] = "127.0.0.1";
    const int PORT = 9992;
    char enviar[1024];
    char buffer[1024];
    int s;
    struct sockaddr_in serveraddr;
    
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0){
        printf("[-] Error when creating socket\n");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &serveraddr.sin_addr); // Converte ip string para binario


    if (connect(s, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0) {
        printf("[-] Error when connecting\n");
        exit(1);
    }

    printf("[+] Connected.\n");
    printf("[+] Say hi!\n");

    while (1){

        printf("Client -> ");
        fgets(enviar, 1024, stdin);
        if (strcmp(enviar, "/exit\n") == 0){ // Receber "comandos"
            break;
        }
        send(s, enviar, strlen(enviar), 0);

        memset(buffer, 0, sizeof(buffer)); // Resetar o que esta salvo no buffer
        int received = recv(s, buffer, sizeof(buffer) -1, 0); // -1 para deixar espaco para o null byte
        if (received <= 0){
            printf("[-] Server disconnected.\n");
            break;
        }
        printf("Server -> %s", buffer);
    }

    printf("[-] Closing socket...\n");
    close(s);

    return 0;
}