#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define NUM_THREADS 2
char sendMessage[1024] ,receiveMessage[1024];

         int sock,result;        

void *sendt(void *threadid)
{
   long tid;
   tid = (long)threadid;
    while(1)
    {
	    //printf("SEND MESSAGE\n :");

	    fgets(sendMessage,512,stdin);
	    
	    send(sock, sendMessage,strlen(sendMessage), 0);
    }



   
   pthread_exit(NULL);
}
void *recdt(void *threadid)
{
   long tid;
   tid = (long)threadid;
    while(1)
    {
            result = recv(sock,receiveMessage,512,0);

receiveMessage[result] = '\0';

printf("Received message : %s \n" , receiveMessage);

                        fflush(stdout);

    }




   pthread_exit(NULL);
}

int main()

{

        //char sendMessage[512],receiveMessage[512];

        //int sock, result; 

        struct hostent *host;

        struct sockaddr_in serverAdd; 

        host = gethostbyname("10.30.7.196");     

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)

            {

            perror("Socket creation failed");

exit(1);

        }

        serverAdd.sin_family = AF_INET;    

        serverAdd.sin_port = htons(5001);  

        serverAdd.sin_addr = *((struct in_addr *)host->h_addr);

        bzero(&(serverAdd.sin_zero),8);

        if (connect(sock, (struct sockaddr *)&serverAdd, sizeof(struct sockaddr)) == -1)

        {

            perror("Connection failed");

            exit(1);

        }
        pthread_t threads[NUM_THREADS];
           int rc;
           long t;
	   
     
      rc = pthread_create(&threads[0], NULL, sendt, NULL);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
      rc = pthread_create(&threads[1], NULL, recdt, NULL);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
      pthread_join(threads[0], NULL);  
  
    // waiting for when thread_two is completed 
      pthread_join(threads[1], NULL); 


        

        return 0;

}
