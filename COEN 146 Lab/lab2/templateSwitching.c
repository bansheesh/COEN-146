/*
* Name: Lydia Martin
* Date: Tuesday 5:15
* Title: Lab 2 – Packet & Circuit Switching
* Description: This program simulates a packet switching network by calculating the 
* probabilities of users transmitting data over a shared link. This allows the network
* to be used as efficiently as possible without needing reserve bandwidth.
*/
//COEN 146L : Lab2 - Steps 2 and 3
#include <stdio.h>  
#include <math.h>
#include <stdlib.h>

//factorial function returns the factorial of the given number
double factorial(double n){
    int fact = 1;
    for(int i = 1;i < n;i++){
        fact *= i;
    }
    return fact;
}

int main(int argc, char *argv[]) {
    if (argc !=5){
	fprintf(stderr,"usage: %s <linkBandwidth> <userBandwidth> <tPSusers> <nPSusers> \n", argv[0]);
	exit(0);
    } 
   int i, linkBandwidth, userBandwidth, nCSusers, nPSusers;
   double pPSusers, tPSusers, pPSusersBusy, pPSusersNotBusy;
   
   // Get values from command line
   linkBandwidth = atoi(argv[1]); 
   userBandwidth = atoi(argv[2]);
   tPSusers  = atof(argv[3]);
   nPSusers = atoi(argv[4]);

   // 2a: Circuit Switching Senario
   // how many users --> nCSusers
   nCSusers = linkBandwidth/userBandwidth;
   printf("2a: Circuit switching senario: %d users\n", nCSusers);
   
   // 3: Packet Switching Senario
   //compute: 3a, b, c, d, e, f, g, h 

   pPSusers = tPSusers; // Probability that a specific user is busy transmitting
   pPSusersNotBusy = 1 - pPSusers; // Probability that a specific user is not busy
   
   //probability that different numbers of users are busy or not busy
   double pAllNotBusy = pow((1 - pPSusers),(nPSusers - 1)); 
   double pOneBusy = pPSusers * pow((pPSusersNotBusy),(nPSusers - 1));
   double pExactlyOne = nPSusers * (pPSusers * pow((pPSusersNotBusy), (nPSusers - 1)));
   double pTenBusy = pow(pPSusers,10) * pow(pPSusersNotBusy,(nPSusers - 10));
   
   //probability that any 10 users are busy 
   double temp = (factorial(nPSusers) / (factorial(10) * factorial(nPSusers - 10)));
   double pAnyTenBusy = temp * pow(pPSusers,10) * pow(pPSusersNotBusy,(nPSusers - 10));
   
   //probability that 10 or more users are busy
   double pTenMoreBusy = 0.0;
   for (int i = 11; i <= nPSusers;i++){
    double binomCoeff = factorial(nPSusers) / (factorial(i) * factorial(nPSusers - i));
    pTenMoreBusy += binomCoeff * pow(pPSusers, i) * pow(pPSusersNotBusy, nPSusers - i);
   }
   
   
   printf("3: Packet switching senario");
   printf("3a: The probability that a given (specific) user is busy transmitting = %f\n", pPSusers);
   printf("3b: The probability that ine specific other user is not busy =  %f\n", pPSusersNotBusy);
   printf("3c: The probablility that all of the other specific other users are not busy =  %e\n", pAllNotBusy);
   printf("3d: The probability that one specific user is transmitting and the remianing users are not transmitting =  %e\n", pOneBusy);
   printf("3e: The probability that exactly one of the nPSusers users is busy is pPSusers times the probability that a given specific user is transmitting and the remaining users are not transmitting = %le\n", pExactlyOne);
   printf("3f: The probabalitiy that 10 specific users of nPSusers are transmitting and the others are idle = %lg\n", pTenBusy);
   printf("3g: The probability that any 10 users of nPSusers are transmitting and the others are idle = %lg\n", pAnyTenBusy);
   printf("3h: The probability that more than 10 users of nPSusers are transmitting and the others are idle = %lg\n", pTenMoreBusy);
   return 0;
}