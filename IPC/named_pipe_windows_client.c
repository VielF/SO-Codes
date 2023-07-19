//Cliente named pipe - Testato no CLion
#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 1024

int main()
{
    HANDLE hPipe;
    char buffer[BUFFER_SIZE];
    DWORD dwRead, dwWritten;

    // Open named pipe
    hPipe = CreateFile("\\\\.\\pipe\\pipeso",
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Falha em abrir o pipe. Codigo do erro: %d\n", GetLastError());
        return 1;
    }

    printf("Conectado ao servidor!\n");

    // Send data to server
    printf("Entre com o dado a ser enviado: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    if (!WriteFile(hPipe, buffer, strlen(buffer) + 1, &dwWritten, NULL))
    {
        printf("Falha em escrever no pipe. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Dado enviado ao servidor.\n");

    // Read data from server
    if (!ReadFile(hPipe, buffer, BUFFER_SIZE, &dwRead, NULL))
    {
        printf("Falha em ler do pipe. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Dado recebido: %s\n", buffer);

    // Close named pipe and exit
    CloseHandle(hPipe);
    return 0;
}