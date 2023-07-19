//Servidor Named Pipe - testado com CLion
#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 1024

int main()
{
    HANDLE hPipe;
    char buffer[BUFFER_SIZE];
    DWORD dwRead;

    // Create named pipe
    hPipe = CreateNamedPipe("\\\\.\\pipe\\pipeso",
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                            1,
                            BUFFER_SIZE,
                            BUFFER_SIZE,
                            0,
                            NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Falha em criar o pipe. Codigo do erro: %d\n", GetLastError());
        return 1;
    }

    // Wait for client to connect
    printf("Esperando o cliente conectar...\n");
    if (!ConnectNamedPipe(hPipe, NULL))
    {
        printf("Falha em conectar ao cliente. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Cliente conectado!\n");

    // Read data from client
    if (!ReadFile(hPipe, buffer, BUFFER_SIZE, &dwRead, NULL))
    {
        printf("Falha em ler do pipe. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Data recebido: %s\n", buffer);

    // Write data back to client
    if (!WriteFile(hPipe, buffer, dwRead, NULL, NULL))
    {
        printf("Falha em escrever no pipe. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Dado enviado de volta ao cliente.\n");

    // Close pipe and exit
    CloseHandle(hPipe);
    return 0;
}