#ifdef _WIN32 // Windows
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#else // unix
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#endif
#include <cctype>
#include <arpa/inet.h>
#include <string.h>

void error(const char *msg)
{
    #ifdef _Win32
    WSACleanup();
    #endif
    perror(msg);
    exit(1);
}

int main(int argc,char **argv)
{
    #ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) error("WSAStartup failed");
    #endif
    if (argc != 3) error("Wrong arguments");
    int sock_type, n;
    if (strcmp(argv[2], "TCP") == 0) sock_type = SOCK_STREAM;
    else if (strcmp(argv[2], "UDP") == 0) sock_type = SOCK_DGRAM;
    else error("Wrong arguments");
    char buffer[256];
    int port = atoi(argv[1]);
    int server_socket, client_socket;
    printf("Creating %s socket on port %s\n", argv[2], argv[1]);
    server_socket = socket(AF_INET, sock_type, 0);
    if (server_socket < 0) error("Couldn't open socket");
    struct sockaddr_in serv_addr, client_addr;
    socklen_t socklen = sizeof(client_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("Couldn't bind socket");
    if (sock_type == SOCK_DGRAM)
    {
        while (1)
        {
            n = recvfrom(server_socket, buffer, 512, 0,(struct sockaddr *) &client_addr, &socklen);
            if (n < 0) error("Couldn't read from socket");
            printf("Received UDP: \"%s\" from %s\n", buffer, inet_ntoa(client_addr.sin_addr));
            sendto(server_socket, "Message received", 16, 0, (struct sockaddr *) &client_addr, socklen);
        }
    }
    listen(server_socket, 5);
    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &socklen);
        if (client_socket < 0) error("Couldn't accept connection");
        n = recv(client_socket, buffer, 255, 0);
        if (n < 0) error("Couldn't read from socket");
        printf("Received TCP: \"%s\" from %s\n", buffer, inet_ntoa(client_addr.sin_addr));
        n = send(client_socket, "Message received", 16, 0);
        if (n < 0) error("Couldn't write to socket");
    }
    close(client_socket);
    close(server_socket);
    return 0;
}