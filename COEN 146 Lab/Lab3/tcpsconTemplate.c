/*Name: Lydia Martin
* Date: October 14, 2024
* Title: Lab3 - Part 4: Concurrent TCP Server
* Description: This program implements a concurrent TCP server that can handle
* multiple client connections simultaneously. File transfers are all done
* by a separate thread, allowing the server to respond to multiple clients 
* at the same time.
*/

//COEN 146L : Lab3, step 4: concurrent TCP server that accepts and responds to multiple client connection requests, each requesting a file transfer
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

//Define the number of clients/threads that can be served
#define N 100
int threadCount = 0;
pthread_t clients[N]; //declaring N threads

//Declare socket and connection file descriptors.
int server_fd;
int connfd;

//Declare receiving and sending buffers of size 10 bytes
char receive_buf[10];
char send_buf[10];

//Declare server address to which to bind for receiving messages and client address to fill in sending address
struct sockaddr_in server_addr, client_addr;
socklen_t client_len = sizeof(client_addr);

//Connection handler (thread function) for servicing client requests for file transfer
void* connectionHandler(void* sock){
   sleep(5);
   int new_sock = *(int*) sock;
   free(sock);

   //declare buffer holding the name of the file from client
   char client_rec_buf [10];

   //Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
   
	//receive name of the file from the client
	bzero(client_rec_buf, sizeof(client_rec_buf));
   int bytes_received = read(new_sock, receive_buf, sizeof(receive_buf) - 1);

   //open file and send to client
   FILE *file = fopen(receive_buf,"rb");
     
   //read file and send to connection descriptor
   int bytes_read;
   while ((bytes_read = fread(send_buf, sizeof(char), sizeof(send_buf), file)) > 0) {
      if(write(new_sock, send_buf, bytes_read) < 0){
         perror("Error writing to socket");
         fclose(file);
         close(new_sock);
         pthread_exit(0);
         return NULL;
      }
   }
   printf("File transfer complete\n");
   
   //close file
   fclose(file);
   //Close connection descriptor
   close(new_sock);
   pthread_exit(0);
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
   if (bind < 0){
      perror("Binding Error");
      close(server_fd);
      exit(1);
   }

   // Server listening to the socket endpoint, and can queue 5 client requests
   listen(server_fd, 5);
   printf("Server listening/waiting for client at port %d\n", ntohs(server_addr.sin_port));
   

   while (1){
      //Server accepts the connection and call the connection handler
      int *new_sock = malloc(sizeof(int));
      *new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
      
      if (*new_sock < 0) {
        perror("Error accepting connection");
        free(new_sock);
        continue; // Go back to waiting for the next connection
    }

      if(pthread_create(&clients[threadCount], NULL, connectionHandler, (void*) new_sock) < 0){
         perror("Unable to create a thread");
         exit(0);
      }
      else{ 
         printf("Thread %d has been created to service client request\n",++threadCount);
         printf("\n");
         
      }
   }
   for(int i = 0; i < threadCount; i++){
         pthread_join(clients[i], NULL);
   }
   return 0;
}