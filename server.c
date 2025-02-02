#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to handle HTTP requests
void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    
    // Read client request
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Received request:\n%s\n", buffer);

    // Prepare HTTP response
    const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body>"
        "<h1>Welcome to My C HTTP Server!</h1>"
        "<p>This server is running successfully.</p>"
        "</body></html>";

    // Send response to client
    write(client_socket, response, strlen(response));
    
    // Close client socket
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connections
    while (1) {
        client_socket = accept(server_socket, 
                               (struct sockaddr *)&client_address, 
                               &client_address_len);
        
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Handle each client connection
        handle_request(client_socket);
    }

    // Close server socket (this line is never reached in this example)
    close(server_socket);

    return 0;
}