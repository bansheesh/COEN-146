/*Name: Lydia Martin
* Date: October 29, 2024
* Title: Lab5 - UDP server with packets that have checksum, sequence number, acknowledgement number
* Description: This program implements a reliable stop and wait protocol over UDP for file transfer.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

//Declare a Header structure that holds, sequence/ acknowledgement number, checksum, and length of a packet
typedef struct {
    int seq_ack;
    int len;
    int cksum;
} Header;

//Declare a packet structure that holds data and header
typedef struct {
    Header header;
    char data[10];
} Packet;

//Declare server address to fill in address of sender
struct sockaddr_in servAddr, clienAddr;

//Calculating the Checksum
/*The simplest checksum algorithm is the so-called longitudinal parity check, 
which breaks the data into "bytes" with a fixed number 8 bits, and then 
computes the exclusive or (XOR) of all those bytes. 
The result is appended to the message as an extra byte*/
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

//Printing received packet, sequence/acknowledgement number, and checksum
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//Server sending ACK to the client
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    Packet packet;	
    packet.header.seq_ack=seqnum;
    packet.header.len=sizeof(packet.data);
	packet.header.cksum=getChecksum(packet);
    //send Ack packet
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);  
    printf("\t Server sending ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
}

//Server receiving Packet from the client
Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;
    while (1) {
        //recv packets from the client
	    int recv_len = recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        if (recv_len < 0){
            perror("Receive failed.");
            continue; //retry
        }
        
        //print received packet
        printPacket(packet);
	    
        //cacluate the checksum
        int e_cksum = getChecksum(packet);
        
        //check the checksum
        if (packet.header.cksum!=e_cksum) {
            printf("\t Server: Bad checksum, expected checksum was: %d\n", e_cksum);
            //resend packet
            serverSend(sockfd, address, *addrlen, !seqnum);
        
        //check the sequence number
        } else if (packet.header.seq_ack != seqnum) {
            printf("\t Server: Bad seqnum, expected sequence number was:%d\n",seqnum);
            //resend packet
            serverSend(sockfd, address, *addrlen, !seqnum);

        //if the ckechsum and sequence numbers are correct send ack with the right sequence number
        } else {
            printf("\t Server: Good packet\n");
            //send ack
            serverSend(sockfd, address, *addrlen, seqnum);
            break;
        }
    }
    return packet;
}

int main(int argc, char *argv[]) {
    //Get from the command line, server IP and dst file
    if (argc != 3) {
        printf("Usage: %s <port> <dstfile>\n", argv[0]);
        exit(0);
    }

    //Declare socket file descriptor
    int sockfd;

    //Open a UDP socket, if successful, returns a descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        perror("Socket creation faieled.");
        exit(1);
    }

    //Setup the server address to bind using socket addressing structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind IP address and port for server endpoint socket
    if (bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        perror("Bind failed.");
        exit(1);
    }

    //open file using argv[2]
    int fp = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fp < 0){
        perror("file open failed.");
        exit(1);
    }
    //initialize sequence number to 0
    int seqnum = 0;
    
    //get file contents (as packets) from client
    Packet packet;
    socklen_t addr_len = sizeof(clienAddr);
    printf("Waiting for packets to come.....\n");
    do {
        //receive a packet and check through serverReceive()
        packet = serverReceive(sockfd, (struct sockaddr *)&clienAddr, &addr_len, seqnum);
        write(fp, packet.data, strlen(packet.data));
        //alternate the sequence number between 0 and 1
        seqnum = (seqnum+1) % 2;
    } while (packet.header.len != 0);
    //close file and socket descriptors
    close(fp);
    close(sockfd);
    return 0;
}
