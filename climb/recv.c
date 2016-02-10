
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define MAXRCVLEN 1024
#define PORTNUM 2300
 
int main()
{
    char buffer[MAXRCVLEN];
  
    struct sockaddr_in dest; /* socket info about the machine connecting to us */
    struct sockaddr_in serv; /* socket info about our server */
    int len, mysocket;            /* socket used to listen for incoming connections */
    socklen_t socksize = sizeof(struct sockaddr_in);

    memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
    serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
    serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
    serv.sin_port = htons(PORTNUM);           /* set the server port number */    

    mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
    /* bind serv information to mysocket */
    bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

    /* start listening, allowing a queue of up to 1 pending connection */
    listen(mysocket, 1);
    int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
  
    
    while(consocket)
    {
        //printf("Incoming connection from %s\n", inet_ntoa(dest.sin_addr));
        len = recv(consocket, buffer, MAXRCVLEN, 0);
        buffer[len] = '\0';
        fprintf(stdout, "%s", buffer); 
        fflush(stdout);
        close(consocket);
        consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
    }


}
