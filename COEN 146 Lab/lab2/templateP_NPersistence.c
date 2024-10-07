/*
* Name: Lydia Martin
* Date: Tuesday 5:15
* Title: Lab2 – DNS Lookup and HTTP Response Time Calculator
* Description: This program simulates the time required for someone to use a URL and retrieve 
* web objects from the server. It does this by calculating the times of different scenarios by utitlizing
* the given RTT times for DNS look up and the RTTHTTP time for object retrieval.
*/

//COEN 146L : Lab2, step 4
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
        if (argc != 2) {  // check correct usage
		fprintf(stderr, "usage: %s <n connections> \n", argv[0]);
		exit(1);
	}
        //Declare and values to n (n connections), np (np parralel connections), RTT0, RTT1, RTT2, RTTHTTP, RTTDNS, .. 
        int n_connections = atoi(argv[1]);
        int RTT0 = 3;
        int RTT1 = 20;
        int RTT2 = 26; 
        int RTTHTTP = 47;
        int RTTDNS = RTT0 + RTT1 + RTT2;
        
        //Part A:
        //total round trip times required for DNS lookup & recieval of HTML object
        int total_RTT = RTTDNS + RTTHTTP; 
        printf("One object: %d msec\n", total_RTT);
        
        //Part B:
        //Every object needs a new connection so we account for each RTT
        int RTT_nPersistent = (RTT0 + RTT1 + RTT2) + 6 * RTTHTTP;
        printf("Non-Persistent 6 objects: %d msec\n", RTT_nPersistent);

        //Part C:
        //find how many np (parralel connections)
        //same time to perform persistent np as the total RTT 
        printf("%d parallel connection - Persistent: %d msec\n", n_connections, total_RTT);
        
        //each object needs a new connection, this line accounts for one additional object fetched
        int np_connections = RTT0 + RTT1 + RTT2 + RTTHTTP + RTTHTTP;
        printf("%d parallel connection - Non-Persistent: %d msec\n", n_connections, np_connections);

return 0;
}