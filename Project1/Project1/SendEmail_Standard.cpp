#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <sys/types.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fmt/core.h>


#define MAX_BUFFER 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock\n");
        return 1;
    }

    const char* smtpServer = "smtp-mail.outlook.com";
    const int smtpPort = 587;

    const char* from = "smtpTMEIC@outlook.com";
    const char* to = "aniruddh.chauhan@tmeic.com";
    const char* subject = "Test Email";
    const char* message = "This is a test email sent via SMTP in C.";

    // Create a socket
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        perror("Error creating socket");
        return 1;
    }

    struct hostent* server = gethostbyname(smtpServer);
    if (server == NULL) {
        perror("Error getting host by name");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(smtpPort);
    serverAddr.sin_addr.s_addr = *(unsigned long*)server->h_addr;

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Error connecting to server");
        return 1;
    }

    char response[MAX_BUFFER];

    // Receive the server's welcome message
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Send the HELO command
    send(sockfd, "HELO localhost\r\n", strlen("HELO localhost\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Send the MAIL FROM command
    send(sockfd, "MAIL FROM:<smtpTMEIC@outlook.com>\r\n", strlen("MAIL FROM:<smtpTMEIC@outlook.com>\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Send the RCPT TO command
    send(sockfd, "RCPT TO:<aniruddh.chauhan@tmeic.com>\r\n", strlen("RCPT TO:<aniruddh.chauhan@tmeic.com>\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Send the DATA command
    send(sockfd, "DATA\r\n", strlen("DATA\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Send the email headers and message
    char emailData[MAX_BUFFER];
    snprintf(emailData, MAX_BUFFER, "From: %s\r\nTo: %s\r\nSubject: %s\r\n\r\n%s\r\n.\r\n", from, to, subject, message);
    send(sockfd, emailData, strlen(emailData), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Send the QUIT command and close the connection
    send(sockfd, "QUIT\r\n", strlen("QUIT\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);
    closesocket(sockfd);

    WSACleanup();

    return 0;
}
