//Servidor pipe (testado usando WSL)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/pipeso"

int main()
{
    int sockfd, newsockfd, len;
    struct sockaddr_un local, remote;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Falha em criar o pipe");
        return 1;
    }

    // Bind socket to local address
    memset(&local, 0, sizeof(local));
    local.sun_family = AF_UNIX;
    strncpy(local.sun_path, SOCK_PATH, sizeof(local.sun_path) - 1);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(sockfd, (struct sockaddr *)&local, len) < 0)
    {
        perror("Falha em capturar o socket");
        close(sockfd);
        return 1;
    }

    // Listen for connections
    if (listen(sockfd, 5) < 0)
    {
        perror("Falha em escutar o socket");
        close(sockfd);
        return 1;
    }

    printf("Servidor Named pipe ouvindo em %s...\n", SOCK_PATH);

    // Accept connections
    memset(&remote, 0, sizeof(remote));
    len = sizeof(remote);
    newsockfd = accept(sockfd, (struct sockaddr *)&remote, &len);
    if (newsockfd < 0)
    {
        perror("Falha em aceitar coneccao");
        close(sockfd);
        return 1;
    }

    printf("Cliente conectado!\n");

    // Read data from client
    if (read(newsockfd, buffer, sizeof(buffer)) < 0)
    {
        perror("Falha em ler do socket");
        close(newsockfd);
        close(sockfd);
        return 1;
    }

    printf("Dado recebido: %s\n", buffer);

    // Process data
    // In this example, we just convert the string to uppercase
    for (int i = 0; i < strlen(buffer); i++)
    {
        buffer[i] = toupper(buffer[i]);
    }

    // Write processed data back to client
    if (write(newsockfd, buffer, strlen(buffer) + 1) < 0)
    {
        perror("Falha em escrever no socket");
        close(newsockfd);
        close(sockfd);
        return 1;
    }

    printf("Dado enviado de volta para o cliente.\n");

    // Close sockets and exit
    close(newsockfd);
    close(sockfd);
    return 0;
}