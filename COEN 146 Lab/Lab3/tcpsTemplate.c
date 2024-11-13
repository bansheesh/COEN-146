/* Name: Lydia Martin
* Date: October 14, 2024
* Title: Lab3 - Part 1: TCP Server
* Description: This program implements TCP server that listens for
* incoming client connections and transfers files.
* The server accepts a connection from a client, receives the
* filename from the client, and sends the file's contents back
* through the established connection.
*/

//COEN 146L : Lab3, step 1: TCP server that accepts a client connection for file transfer.
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

//Declare socket file descriptor.
int server_fd;

//Declare receiving and sending buffers of size 10 bytes
char receive_buf[10];
char send_buf[10];

//Declare server address to which to bind for receiving messages and client address to fill in sending address
struct sockaddr_in server_addr, client_addr;
socklen_t client_len = sizeof(client_addr);

//Connection handler for servicing client file transfer request
void* connectionHandler(void* sock){
   //declare buffer holding the name of the file from client
   char client_rec_buf [10];
   //get the connection descriptor
   int new_sock = *(int*)sock;
   free(sock);

   //Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
   
	//receive name of the file from the client
	//Clear the receive buffer to ensure it's empty
   bzero(client_rec_buf, sizeof(client_rec_buf)); 
   int bytes_received = read(new_sock, client_rec_buf, sizeof(client_rec_buf) - 1);
   
   // Print the received filename to check if it's correct
   printf("Received filename: %s\n", client_rec_buf);

   //open file 
   FILE *file = fopen(client_rec_buf, "r"); // Open requested file in binary read mode
   if (file == NULL) {
    perror("Error opening file");
    close(new_sock);
    return NULL; // Handle the error
}

   //read file and send to connection descriptor
   int bytes_read;
   while ((bytes_read = fread(send_buf, sizeof(char), sizeof(send_buf), file)) > 0) {
      if(write(new_sock, send_buf, bytes_read) < 0){
         perror("Error writing to socket");
         fclose(file);
         close(new_sock);
         return NULL;
      }
   }
   printf("File transfer complete\n");
   
   //close file
    fclose(file);
   //Close connection descriptor
   close(new_sock);

   return 0;
}


int main(int argc, char *argv[]){
   //Get from the command line, server IP, src and dst files.
   if (argc != 2){
      printf ("Usage: %s <port #> \n",argv[0]);
      exit(0);
   } 
   //Open a TCP socket, if successful, returns a descriptor
   server_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (server_fd < 0){
      perror("Error oepening socket");
      exit(1);
   }
   //Setup the server address to bind using socket addressing structure
   bzero((char *) &server_addr, sizeof(server_addr)); // Clear the server address structure
   server_addr.sin_family = AF_INET; // Set the address family to IPv4
   server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
   server_addr.sin_port = htons(atoi(argv[1])); // Convert the port number from host to network byte order
   
   //bind IP address and port for server endpoint socket 
   bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

   // Server listening to the socket endpoint, and can queue 5 client requests
   listen(server_fd, 5);
   printf("Server listening/waiting for client at port %d\n", ntohs(server_addr.sin_port));

   //Server accepts the connection and call the connection handler
   while (1){
      int *new_sock = malloc(sizeof(int));
      *new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
      if(*new_sock < 0){
         perror("Error accepting connection");
         free(new_sock);
         continue;
      }
      connectionHandler(new_sock);
      break;
   }
 
   //close socket descriptor
   printf("Server Closed \n");
   close(server_fd);

   return 0;
}