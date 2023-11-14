//#include <stdio.h>
//#include <string.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>
//#include <openssl/rand.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//
//#define SMTP_SERVER "smtp.example.com"
//#define SMTP_PORT 587
//#define SENDER_EMAIL "your_email@example.com"
//#define SENDER_PASSWORD "your_password"
//#define RECIPIENT_EMAIL "recipient@example.com"
//#define EMAIL_SUBJECT "Test Email"
//#define EMAIL_BODY "This is a test email sent from C."
//
//int main() {
//    // Initialize the OpenSSL library
//    SSL_library_init();
//    SSL_load_error_strings();
//    ERR_load_BIO_strings();
//    OpenSSL_add_all_algorithms();
//
//    // Create a SSL context and a socket
//    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
//    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    struct sockaddr_in server_address;
//    server_address.sin_family = AF_INET;
//    server_address.sin_port = htons(SMTP_PORT);
//    server_address.sin_addr.s_addr = inet_addr(SMTP_SERVER);
//
//    // Connect to the SMTP server
//    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
//        perror("Error connecting to the SMTP server");
//        return 1;
//    }
//
//    // Create an SSL object and bind it to the socket
//    SSL* ssl = SSL_new(ctx);
//    SSL_set_fd(ssl, sockfd);
//
//    // Connect the SSL object to the SMTP server
//    if (SSL_connect(ssl) != 1) {
//        ERR_print_errors_fp(stderr);
//        SSL_free(ssl);
//        return 1;
//    }
//
//    // Send the EHLO command
//    SSL_write(ssl, "EHLO localhost\r\n", strlen("EHLO localhost\r\n"));
//
//    // Read and print the server's response
//    char response[1024];
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Login to the SMTP server
//    SSL_write(ssl, "AUTH LOGIN\r\n", strlen("AUTH LOGIN\r\n"));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Send the base64-encoded username
//    SSL_write(ssl, SENDER_EMAIL, strlen(SENDER_EMAIL));
//    SSL_write(ssl, "\r\n", strlen("\r\n"));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Send the base64-encoded password
//    SSL_write(ssl, SENDER_PASSWORD, strlen(SENDER_PASSWORD));
//    SSL_write(ssl, "\r\n", strlen("\r\n"));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Send the MAIL FROM command
//    char mail_from_command[256];
//    snprintf(mail_from_command, sizeof(mail_from_command), "MAIL FROM: <%s>\r\n", SENDER_EMAIL);
//    SSL_write(ssl, mail_from_command, strlen(mail_from_command));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Send the RCPT TO command
//    char rcpt_to_command[256];
//    snprintf(rcpt_to_command, sizeof(rcpt_to_command), "RCPT TO: <%s>\r\n", RECIPIENT_EMAIL);
//    SSL_write(ssl, rcpt_to_command, strlen(rcpt_to_command));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Send the DATA command
//    SSL_write(ssl, "DATA\r\n", strlen("DATA\r\n"));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Send the email subject and body
//    char email_subject[256];
//    snprintf(email_subject, sizeof(email_subject), "Subject: %s\r\n", EMAIL_SUBJECT);
//    SSL_write(ssl, email_subject, strlen(email_subject));
//    SSL_write(ssl, "\r\n", strlen("\r\n"));
//    SSL_write(ssl, EMAIL_BODY, strlen(EMAIL_BODY));
//    SSL_write(ssl, "\r\n.\r\n", strlen("\r\n.\r\n"));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Send the QUIT command
//    SSL_write(ssl, "QUIT\r\n", strlen("QUIT\r\n"));
//
//    // Read and print the server's response
//    SSL_read(ssl, response, sizeof(response));
//    printf("%s", response);
//
//    // Clean up and close the connection
//    SSL_free(ssl);
//    close(sockfd);
//    SSL_CTX_free(ctx);
//
//    return 0;
//}
