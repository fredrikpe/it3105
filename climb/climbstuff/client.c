#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define MAXRCVLEN 500
#define PORTNUM 2300
 
int main(int argc, char *argv[])
{
   char *msg = "Hello World!\n";

   char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
   int len, mysocket;
   struct sockaddr_in dest; 
 
   mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = inet_addr("10.20.60.82"); //INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(PORTNUM);                /* set destination port number */
 
   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
  
   //len = recv(mysocket, buffer, MAXRCVLEN, 0);
   send(mysocket, msg, strlen(msg), 0);

 
   /* We have to null terminate the received data ourselves */
   //buffer[len] = '\0';
 
   //printf("Sent msg"); //Received %s (%d bytes).\n", buffer, len);
 
   close(mysocket);
   return EXIT_SUCCESS;
}
