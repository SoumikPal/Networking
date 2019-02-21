#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
struct client_info {
	int sockno;
	char ip[INET_ADDRSTRLEN];
};
int clients[100];
int chas[1000];
int n = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void sendtop(char *msg,int curr,int fd)
{
	int i;
	pthread_mutex_lock(&mutex);
	/*for(i = 0; i < n; i++) {
		if(clients[i] != curr) {
			if(send(clients[i],msg,strlen(msg),0) < 0) {
				perror("sending failure");
				continue;
			}
		}
	}*/
    if(clients[fd]!=0)
    {
         send(clients[fd],msg,strlen(msg),0);
    }
    else
    {
          char mf[100]="Receiver has already left\n";
          send(curr,mf,strlen(mf),0);
    }
          
	pthread_mutex_unlock(&mutex);
}
void *recvmg(void *sock)
{
	struct client_info cl = *((struct client_info *)sock);
	char msg[500];
	int len;
	int i;
	int j;
	while((len = recv(cl.sockno,msg,500,0)) > 0) {
        int ii=0;
        int cc=2;
        char mmesg[1000];
		msg[len] = '\0';
        int fd=msg[0]-'0';
        while(msg[cc])
        {
            mmesg[ii++]=msg[cc];
            cc++;
        }
        mmesg[ii]='\0';
		sendtop(mmesg,cl.sockno,fd);
		memset(msg,'\0',sizeof(msg));
	}
	pthread_mutex_lock(&mutex);
	printf("%d disconnected\n",cl.sockno);
	/*for(i = 0; i < n; i++) {
		if(clients[i] == cl.sockno) {
			j = i;
			while(j < n-1) {
				clients[j] = clients[j+1];
				j++;
			}
		}
	}*/
    clients[chas[cl.sockno]]=0;
	//n--;
	pthread_mutex_unlock(&mutex);
}
int main(int argc,char *argv[])
{
    memset(clients,0,sizeof(clients));
	struct sockaddr_in my_addr,their_addr;
	int my_sock;
	int their_sock;
	socklen_t their_addr_size;
	int portno;
	pthread_t sendt,recvt;
	char msg[500];
	int len;
	struct client_info cl;
	char ip[INET_ADDRSTRLEN];;
	;
	if(argc > 2) {
		printf("too many arguments");
		exit(1);
	}
	portno = atoi(argv[1]);
	my_sock = socket(AF_INET,SOCK_STREAM,0);
	memset(my_addr.sin_zero,'\0',sizeof(my_addr.sin_zero));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(portno);
	//my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    my_addr.sin_addr.s_addr=INADDR_ANY;  
	their_addr_size = sizeof(their_addr);

	if(bind(my_sock,(struct sockaddr *)&my_addr,sizeof(my_addr)) != 0) {
		perror("binding unsuccessful");
		exit(1);
	}

	if(listen(my_sock,5) != 0) {
		perror("listening unsuccessful");
		exit(1);
	}

	while(1) {
		if((their_sock = accept(my_sock,(struct sockaddr *)&their_addr,&their_addr_size)) < 0) {
			perror("accept unsuccessful");
			exit(1);
		}
		pthread_mutex_lock(&mutex);
		inet_ntop(AF_INET, (struct sockaddr *)&their_addr, ip, INET_ADDRSTRLEN);
		printf("%s connected\n",ip);
		cl.sockno = their_sock;
		strcpy(cl.ip,ip);
        printf("%d\n",their_sock);
		clients[n] = their_sock;
        chas[their_sock]=n;
        
		n++;
		pthread_create(&recvt,NULL,recvmg,&cl);
		pthread_mutex_unlock(&mutex);
	}
	return 0;
}
