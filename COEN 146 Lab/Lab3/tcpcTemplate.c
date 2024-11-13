/*Name: Lydia Martin
* Date: October 14, 2024
* Title: Lab3 - Part 2: TCP Client
* Description: This program implements a TCP client that connects to a server
* to request a file transfer. The client sends the name of the requested 
* file to the server and receives the file data, writing it to a specified destination file.
*/
//COEN 146L : Lab3, step 2: TCP client requesting a file transfer from the server.
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
  //Get from the command line, server IP, src and dst files.
  if (argc != 5){
  printf ("Usage: %s <ip of server> <port #> <src_file> <dest_file>\n",argv[0]);
  exit(0);
  } 
  //Declare socket file descriptor and buffer
  int sockfd;
  char buf[10];
  
  //Declare server address to accept
  struct sockaddr_in serv_addr;

  //Declare host
  struct hostent *server;

  //get hostname
  server = gethostbyname(argv[1]);

  //Open a socket, if successful, returns
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    perror("Error opening socket.");
    exit(1);
  }

  //Set the server address to send using socket addressing structure
  bzero((char *) &serv_addr, sizeof(serv_addr));//Zero out the serv_addr structure to clear any garbage values
  serv_addr.sin_family = AF_INET; //Set the address family to IPv4
  bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr, server->h_length); //Copy the server's IP address into the sin_addr.s_addr field
  serv_addr.sin_port = htons(atoi(argv[2])); //Convert the port number from a string to an integer and set it in network byte order

  //Connect to the server using socket file descriptor and server address
  int connection_status = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (connection_status < 0){
    perror("Error connecting to server");
    exit(1);
  }

  //Client sends the name of the file to retrieve from the server
  size_t filename_length = strlen(argv[3]);
  //write the file name to the socket
  int b_written = write(sockfd, argv[3], filename_length);
  if (connection_status < 0){
    perror("Error writing to socket");
    exit(1);
  }

  //Client begins to write and read from the server
  FILE *dst_file = fopen(argv[4], "w");
  //read socket data into buffer
  int n;
  while ((n = read(sockfd, buf, sizeof(buf))) > 0) {
    //write data from buffer into destination file
    if(fwrite(buf, 1, n, dst_file) < n){
      perror("Error Writing to File.");
      close(sockfd);
      fclose(dst_file);
      exit(1);
    }
  }

  //Close socket descriptor
  printf("client sockets and dest file closed.\n");
  close(sockfd);
  fclose(dst_file);
  
  return 0;
}

