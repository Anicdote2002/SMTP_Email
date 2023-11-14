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

    const char* smtpServer = "smtp.example.com";
    const int smtpPort = 25;

    const char* from = "your_email@example.com";
    const char* to = "recipient@example.com";
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

    // Send the EHLO command
    send(sockfd, "EHLO localhost\r\n", strlen("EHLO localhost\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Check if the server supports STARTTLS
    if (strstr(response, "STARTTLS") == NULL) {
        printf("STARTTLS is not supported by the server.\n");
        return 1;
    }

    // Send the STARTTLS command
    send(sockfd, "STARTTLS\r\n", strlen("STARTTLS\r\n"), 0);
    recv(sockfd, response, MAX_BUFFER, 0);
    printf("%s\n", response);

    // Upgrade the socket to an SSL connection
    SSL_library_init();
    SSL_CTX* ssl_ctx = SSL_CTX_new(SSLv23_client_method());
    SSL* ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // Continue with sending the email as shown in the previous example

    // ...

    // Cleanup and close the SSL connection
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ssl_ctx);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
