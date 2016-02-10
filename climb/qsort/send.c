
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
#define SERVER "192.168.1.18" 
 
 
void client(char* data, int length)
{
   int len, mysocket;
   struct sockaddr_in dest; 
 
   mysocket = socket(AF_INET, SOCK_STREAM, 0);
  
   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = inet_addr(SERVER); //INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(PORTNUM);                /* set destination port number */
 
   connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
  
   send(mysocket, data, length, 0);

   //printf("Sent msg\n"); //Received %s (%d bytes).\n", buffer, len);
   
   close(mysocket);
}
int cmpfunc (const void * a, const void * b)
{
       return ( *(int*)a - *(int*)b );
}
int main()
{
    char *line = NULL;
    size_t size;
    int read;
    int p;
    int *nums = (int*)malloc(10000000*sizeof(int));
    int c = 0;
    while ((read = getline(&line, &size, stdin)) != -1) {
        if (*line == 'p') {
            p = atoi(&(line[2]));
            //printf("%d\n", p);
        } else if (*line == 'q') {
            char *tok = strtok(line, " ");
            tok = strtok(NULL, " ");
            while (tok != NULL) {
                nums[c++] = atoi(tok);
                tok = strtok(NULL, " ");
            }
        }
    } 

    qsort(nums, c, sizeof(int), cmpfunc);
    
    printf("s ");
    int j;
    for (j = 0; j<c; j++) {
        printf("%d ", nums[j]);
    }
    printf("\n");
    free(nums);
    return 0;
}
