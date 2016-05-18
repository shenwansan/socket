#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "packet.h"
#include "socket_log.h"

#define S_PORT 8899
#define S_IP "172.16.1.212"

static int end_flag;

void *thread_read(void * argc)
{
	pthread_detach(pthread_self());
	int sockfd = *(int *) argc;
	while(TRUE)
	{
		int recnum;
		char buffer[PACKET_1024] ;
		memset(buffer, PACKET_0, PACKET_1024);
		if((recnum=read(sockfd,buffer,PACKET_1024))<PACKET_0)
		{
			soc_write("nread error\n");
			goto ERR;
		}
		else
		{
			PACKET_HEAD *packet = (PACKET_HEAD *) malloc (sizeof(PACKET_HEAD));
			memset(packet, PACKET_0, sizeof(PACKET_HEAD));
			memcpy(packet,buffer,sizeof(packet->head) + sizeof( packet->type) + sizeof( packet->len));
			if(packet->len > PACKET_0)
			{
				packet->data = (char *)calloc(packet->len,sizeof(char));
				memcpy(packet->data, buffer + sizeof(packet->head) + sizeof( packet->type) + sizeof( packet->len),packet->len );
				printf("client recv:%s\n",(char *)packet->data);
			
			
				if(!memcmp((char *)packet->data,"bye",PACKET_3)|| !recnum)
				{
					end_flag = PACKET_1;
					printf("server say bye to you,please type any keyboard\n");
					break;
				}
				printf("Recv Message from server(%d)) :%s\n",packet->len,(char *)packet->data);
			}
		}
	}
ERR:
	pthread_exit(NULL);
}

void *thread_write(void * argc)
{
	pthread_detach(pthread_self());
	int sockfd = *(int *) argc;
	char buffer[PACKET_1024] = {PACKET_0,};
	while(PACKET_1)
	{
		if(end_flag)
		{
			sprintf(buffer,"bye");
			write(sockfd,buffer,strlen(buffer)); 
			sleep(PACKET_1);
			end_flag = PACKET_2;
			break;
		}
		else
		{
			PACKET_HEAD *packet = (PACKET_HEAD *) malloc (sizeof(PACKET_HEAD));
			memset(packet, PACKET_0, sizeof(PACKET_HEAD));
			printf("please input your message\n");
			fgets(buffer, PACKET_1024, stdin);
			packet->head = PAC_HEAD;
			packet->type = PACKET_1;
			packet->len = strlen(buffer);
			packet->data = (char *)malloc(packet->len );
			memset(packet->data, PACKET_0, packet->len);
			packet->crc = PACKET_0;
			packet->end = PAC_END;
			memcpy(packet->data, buffer, packet->len );
			char * buffer_str = (char *)calloc(sizeof(PACKET_HEAD) + packet->len, sizeof(char));
			memcpy(buffer_str, packet, sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len));
			memcpy(buffer_str + sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len), packet->data, packet->len);
			memcpy(buffer_str + sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len) + packet->len, &(packet->crc), sizeof( packet->crc));
			memcpy(buffer_str + sizeof(packet->head) + sizeof(packet->type) + sizeof(packet->len) + packet->len + sizeof(packet->crc), &(packet->end), sizeof( packet->end));
			write(sockfd,buffer_str,sizeof(PACKET_HEAD) + (packet->len)); 
			free(buffer_str);
			free(packet->data);
			free(packet);
			sleep(PACKET_1);
		}
	}
	pthread_exit(NULL);
}


int sc_client_init()
{
	int ret ;
	if((ret = soc_init()) < PACKET_0)
	{
		soc_write("sc_client_init error\n");
		return PACKET_ERROR;
	}
	return PACKET_0;
}

int main()
{
	int  sockfd;
	
	pthread_t pthread_r,pthread_w;
	struct sockaddr_in server_in;
	memset(&(server_in),PACKET_0,sizeof(struct sockaddr));
	
	sc_client_init();
	if((sockfd = socket(AF_INET,SOCK_STREAM,PACKET_0)) < PACKET_0)
	{
		soc_write("\nsocket error\n");
		goto ERR;
	}
	server_in.sin_family = AF_INET;
	server_in.sin_addr.s_addr=inet_addr(S_IP);
	server_in.sin_port = htons(S_PORT);
	
	if((connect(sockfd, (struct sockaddr * )&server_in, sizeof(struct sockaddr))) < PACKET_0)
	{
		soc_write("nbind error\n");
		goto ERR;
	}
	
	if(pthread_create(&pthread_r,NULL,thread_read,(void *)&sockfd) < PACKET_0)
	{
		soc_write("\ncreate read thread error\n");
		goto ERR;
	}
	
	if(pthread_create(&pthread_w,NULL,thread_write,(void *)&sockfd) < PACKET_0)
	{
		soc_write("\ncreate read thread error\n");
		goto ERR;
	}
	
	while(end_flag != PACKET_2);
	
	close(sockfd);
	soc_end();
	return PACKET_0;
ERR:
	if(sockfd > PACKET_0)
		close(sockfd);
	soc_end();	
	return PACKET_ERROR;
}