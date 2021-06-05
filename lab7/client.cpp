#ifdef _WIN32 // Windows
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <ws2tcpip.h>
#else // unix
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <cctype>
#include <stdio.h>
#include <string.h>

void error(const char* msg)
{
#ifdef _Win32
    WSACleanup();
#endif
    perror(msg);
    exit(1);
}

int main(int argc, char** argv)
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) error("WSAStartup failed");
#endif
    if (argc != 4) error("Wrong arguments");
    int sock_type, n;
    if (strcmp(argv[3], "TCP") == 0) sock_type = SOCK_STREAM;
    else if (strcmp(argv[3], "UDP") == 0) sock_type = SOCK_DGRAM;
    else error("Wrong arguments");
    //hostent* addr = gethostbyname(argv[1]);
    //if (addr == NULL) error("Host not found");
    int port = atoi(argv[2]);
    int sockfd;
    printf("Creating %s socket\n", argv[3]);
    sockfd = socket(AF_INET, sock_type, 0);
    if (sockfd < 0) error("Couldn't open socket");
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    //memcpy((char *) &serv_addr, (char *) &addr, sizeof(addr));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(port);
    socklen_t serv_addr_len = sizeof(serv_addr);
    if (sock_type == SOCK_DGRAM)
    {
            char message[256] = "UDP message";
            n = sendto(sockfd, message, sizeof(message), 0, (struct sockaddr*)&serv_addr, serv_addr_len);
            if (n < 0) error("Couldn't write to socket");
            n = recvfrom(sockfd, message, 256, 0, (struct sockaddr*) &serv_addr, &serv_addr_len);
            if (n < 0) error("Couldn't read from socket");
            printf("Server %s replies: %s\n", inet_ntoa(serv_addr.sin_addr), message);
    }
    else
    {
        printf("Connecting to %s:%s using %s\n", inet_ntoa(serv_addr.sin_addr), argv[2], argv[3]);
        if (connect(sockfd, (struct sockaddr*)&serv_addr, serv_addr_len) < 0) error("Couldn't connect");
        char message[256] = "TCP message";
        n = send(sockfd, message, sizeof(message), 0);
        if (n < 0) error("Couldn't write to socket");
        n = recv(sockfd, message, 255, 0);
        if (n < 0) error("Couldn't read from socket");
        printf("Server %s replies: %s\n", inet_ntoa(serv_addr.sin_addr), message);
    }
    #ifdef _WIN32
    closesocket(sockfd);
    #else
    close(sockfd);
    #endif
    
    return 0;
}