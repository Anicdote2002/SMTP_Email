#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <sys/types.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

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
    const char* to = "aniruddh.chauhan@tmeic.com.com";
    const char* subject = "Test Email";
    const char* message = "This is a test email sent via SMTP in C with STARTTLS using OpenSSL.";

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

    // Send the EHLO command to identify STARTTLS support
    send(sockfd, "EHLO localhost\r\n", strlen("EHLO localhost\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Check if STARTTLS is supported in the response
    if (strstr(response, "STARTTLS") == NULL) {
        fprintf(stderr, "STARTTLS is not supported by the SMTP server.\n");
        return 1;
    }

    // Send the STARTTLS command
    send(sockfd, "STARTTLS\r\n", strlen("STARTTLS\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Initialize OpenSSL
    SSL_library_init();
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    SSL_connect(ssl);

    // Send the EHLO command again over the secured connection
    SSL_write(ssl, "EHLO localhost\r\n", strlen("EHLO localhost\r\n"));
    SSL_read(ssl, response, MAX_BUFFER);
    printf("%s\n", response);

    // Send the MAIL FROM command
    char mailFrom[MAX_BUFFER];
    snprintf(mailFrom, MAX_BUFFER, "MAIL FROM:<%s>\r\n", from);
    SSL_write(ssl, mailFrom, strlen(mailFrom));
    SSL_read(ssl, response, MAX_BUFFER);
    printf("%s\n", response);

    // Send the RCPT TO command
    char rcptTo[MAX_BUFFER];
    snprintf(rcptTo, MAX_BUFFER, "RCPT TO:<%s>\r\n", to);
    SSL_write(ssl, rcptTo, strlen(rcptTo));
    SSL_read(ssl, response, MAX_BUFFER);
    printf("%s\n", response);

    // Send the DATA command
    SSL_write(ssl, "DATA\r\n", strlen("DATA\r\n"));
    SSL_read(ssl, response, MAX_BUFFER);
    printf("%s\n", response);

    // Send the email headers and message
    char emailData[MAX_BUFFER];
    snprintf(emailData, MAX_BUFFER, "From: %s\r\nTo: %s\r\nSubject: %s\r\n\r\n%s\r\n.\r\n", from, to, subject, message);
    SSL_write(ssl, emailData, strlen(emailData));
    SSL_read(ssl, response, MAX_BUFFER);
    printf("%s\n", response);

    // Send the QUIT command and close the connection
    SSL_write(ssl, "QUIT\r\n", strlen("QUIT\r\n"));
    SSL_read(ssl, response, MAX_BUFFER);
    printf("%s\n", response);
    closesocket(sockfd);

    WSACleanup();

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return 0;
}
