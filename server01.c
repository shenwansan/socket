#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/un.h>  
#include <sys/time.h>  
#include <sys/ioctl.h>  
#include <unistd.h>  
#include <netinet/in.h>  
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

#include "packet.h"
#include "socket_log.h"

#define SERVPORT 8899  
#define BACKLOG 10  
#define MAX_CONNECTED_NO 10  //select
#define MAXDATASIZE 4096  

#define EPOLL_RUN_TIMEOUT -1 //epoll timeout
#define EPOLL_SIZE 10000 //epoll client num

#define MAXLINE 4096

void * thread_server(void * argc)
{
	char buf[PACKET_1024] = {PACKET_0,};
	pthread_detach(pthread_self());
	int socketfd = PACKET_0;
	int rsockfd = PACKET_0;
	char buffer[PCAKET_256] = {PACKET_0,};
	int i = PACKET_0;
	int n = PACKET_0;
	socketfd = *(int *) argc;
	while(PACKET_1)
	{
		printf("EPOLLIN\n");
		if ( (rsockfd = socketfd) < PACKET_0)
			break;
		if ( (n = read(rsockfd, buf, MAXDATASIZE)) < PACKET_0) {
			if (errno == ECONNRESET) {
				close(rsockfd);
				socketfd = PACKET_ERROR;
				break;
			} else
				printf("readline error\n");
		} else if (n == PACKET_0) {
			close(rsockfd);
			rsockfd = PACKET_ERROR;
			break;
		}
		PACKET_HEAD *packet = (PACKET_HEAD *) malloc (sizeof(PACKET_HEAD));
		memset(packet, PACKET_0, sizeof(PACKET_HEAD));
		
		memcpy(packet,buf,sizeof(packet->head) + sizeof( packet->type) + sizeof( packet->len));
		
		
		if(packet->len > PACKET_0)
		{
			packet->data = (char *)calloc(packet->len,sizeof(char));
			memcpy(packet->data, buf + sizeof(packet->head) + sizeof( packet->type) + sizeof( packet->len),packet->len );
			printf("AFTER EPOLLIN:%s\n",(char *)packet->data);
			
			char * buffer_str = (char *)calloc(sizeof(PACKET_HEAD) + packet->len, sizeof(char));
			memcpy(buffer_str, packet, sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len));
			memcpy(buffer_str + sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len), packet->data, packet->len);
			memcpy(buffer_str + sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len) + packet->len, &(packet->crc), sizeof( packet->crc));
			memcpy(buffer_str + sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len) + packet->len + sizeof(packet->crc), &(packet->end), sizeof( packet->end));
			write(rsockfd,buffer_str,sizeof(PACKET_HEAD) + (packet->len)); 
			free(buffer_str);
			free(packet->data);
			free(packet);
		}	
		else
		{
			printf("recv error\n");
			write(rsockfd, "recv error", PACKET_10);
		}
	}
	pthread_exit(NULL);
	
}

	//函数：设置sock为non-blocking mode  
void setSockNonBlock(int sock)   
{  
	int flags;  
	flags = fcntl(sock, F_GETFL, PACKET_0);   
	if (flags < PACKET_0) {  
		perror("fcntl(F_GETFL) failed");  
		exit(EXIT_FAILURE);  
	}  
	if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < PACKET_0) {  
		perror("fcntl(F_SETFL) failed");  
		exit(EXIT_FAILURE);  
	}
}

int main()  
{  
	int sin_size,recvbytes;  
	fd_set readfd;  
	fd_set writefd;  
	int i,epfd, nfds,n;
	struct epoll_event event;
	struct epoll_event events[PACKET_20];
	int sockfd,client_fd,rsockfd,wsockfd;
	struct sockaddr_in server_sockaddr,client_sockaddr;  
	int yes = PACKET_1;
	char buf[MAXDATASIZE]; 
	char send_str[MAXDATASIZE]; 

	epfd = epoll_create(PCAKET_256);
	/*创建socket*/  
	if((sockfd = socket(AF_INET,SOCK_STREAM,PACKET_0))==PACKET_ERROR){  
		perror("socket");  
		exit(PACKET_1);  
	}  
	printf("socket success!,sockfd=%d\n",sockfd);  
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) 
	{  
		perror("setsockopt failed");  
		exit(EXIT_FAILURE);  
	}  

	//设置sock为non-blocking  
	setSockNonBlock(sockfd);

	event.data.fd = sockfd;
	event.events = EPOLLIN|EPOLLET;

	if(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd,&event)< PACKET_0)
	{  
		printf("epoll add fail : fd = %d\n", sockfd);  
		return PACKET_ERROR;  
	}
	/*设置sockaddr结构*/
	bzero(&server_sockaddr, sizeof(server_sockaddr));
	server_sockaddr.sin_family=AF_INET;  
	server_sockaddr.sin_port=htons(SERVPORT);  
	server_sockaddr.sin_addr.s_addr=INADDR_ANY;  
	bzero(&(server_sockaddr.sin_zero),PACKET_8);  
	/*将本地ip地址绑定端口号*/  
	if(bind(sockfd,(struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr))==-1){  
		perror("bind");  
		exit(PACKET_1);  
	}  
	printf("bind success!\n");  
	/*监听*/  
	if(listen(sockfd,BACKLOG)== PACKET_ERROR){  
		perror("listen");  
		exit(PACKET_1);  
	}  
	printf("listening....\n");  

	/*select*/  
	while(PACKET_1){
		int timeout = 3000;  
		nfds = epoll_wait(epfd, events, PACKET_20, timeout);
		
		if(nfds < PACKET_0){  
			if(errno == EINTR && epfd > PACKET_0){  
				usleep(1*1000);  
				continue;  
			}  
			printf("epoll wait failed\n");  
			break;  
		}else if(nfds == PACKET_0){  
			//fprintf(stderr, "no socket ready for read within %d secs\n",   
			//		timeout/1000);  
			sleep(1);  
			continue;  
		}  
		
		for(i = PACKET_0; i < nfds; ++i)
		{
			if((events[i].events & EPOLLERR) ||  
					(events[i].events & EPOLLHUP) ||  
					!(events[i].events & EPOLLIN))  
			{  
				//由于events[i].events使用每个bit表示event，因此判断是否包含某个具体事件可以使用`&`操作符   
				close(events[i].data.fd); 
				events[i].data.fd = PACKET_ERROR;				  
			} 
			
			if(events[i].data.fd == sockfd)
			{
				sin_size=sizeof(struct sockaddr_in);
				if((client_fd=accept(sockfd,(struct sockaddr *)&client_sockaddr,&sin_size))==PACKET_ERROR)
				{   //client_sockaddr：客户端地址  
				perror("accept");  
				exit(1);  
				}  
				char *str = inet_ntoa(client_sockaddr.sin_addr);
				printf("accapt a connection from %s\n", str);

				event.data.fd=client_fd;
				event.events=EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,client_fd,&event);
			}
			else if (events[i].events&EPOLLIN){
			
				pthread_t ThreadId;
				
				if(pthread_create(&ThreadId, NULL, thread_server, (void *)&(events[i].data.fd)) == -1)
				{
					perror("pthread create error");
					exit(-1);
				}
				//pthread_join(ThreadId, NULL);			
				} 			
		}
	}
}	
