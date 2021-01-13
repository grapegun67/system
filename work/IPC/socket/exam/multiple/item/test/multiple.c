#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/epoll.h>



#define PATHNAME	"abcd"
#define	LENGTH		1024

typedef struct Client_info{

	int client_sockfd;
	pthread_t pt;

}Client_info;


typedef struct Client_node{

	Client_info data;	
	struct client_node *next;

}Client_node;


typedef struct Sockfd{
	int *client_sockfd;
	int *sockfd;
}Sockfd;


/* head_list */
Client_node *client_head_list; 


void insert_node(Client_node *head, int client_sockfd)
{
	Client_node *new_node = (Client_node *)malloc(sizeof(Client_node));
	
	new_node->data.client_sockfd = client_sockfd;
	new_node->next = head->next;
	head->next = new_node;
}


void print_node_list(Client_node *head)
{
	Client_node *tmp_node = head->next;

	printf("client list: ");

	while(1){

		if(tmp_node != NULL){
			printf("%d, ", tmp_node->data.client_sockfd);
			tmp_node = tmp_node->next;
		}

		else if(tmp_node == NULL){
			break;
		}
	}
	printf("\n");
}


int explore_node(int num)
{
	Client_node *node = client_head_list->next;	

	while(1){

		if(node->data.client_sockfd == num){
			return num;
		}

		else if(node->next == NULL){
			return -1;
		}
	}
}


void *fight_pthread(void *args)
{



}

void *accept_thread(void *args)
{
	char buf[LENGTH];
	Sockfd *sfd = (Sockfd *)args;

	while(1){

 		*(sfd->client_sockfd) = accept(*(sfd->sockfd), NULL, NULL);
                if(*(sfd->client_sockfd) == -1){
               	        perror("accept()-error\n");
       	                return 0;
                }

 		printf("client connected\n");
		
       	        insert_node(client_head_list, *(sfd->client_sockfd));
       	        print_node_list(client_head_list);


		memset(buf, 0, sizeof(buf));
		strncpy(buf, "2*3 = ?", sizeof(buf));
		send(*(sfd->client_sockfd), buf, sizeof(buf), 0);
	}
}


int main(int argc, char **argv)
{
	char buf[LENGTH];

	pthread_t accept_pt;
	Sockfd structfd;
	struct epoll_event event;

	/* head_list initialize */

	client_head_list = (Client_node *)malloc(sizeof(Client_node));
	client_head_list->next = NULL;

	/* initiallize variable  */

	int sockfd, client_sockfd, epfd, ret;
	struct sockaddr_un sockaddr;

	/* memory setting */

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sun_family = AF_UNIX;
	strncpy(sockaddr.sun_path, PATHNAME, sizeof(sockaddr.sun_path)-1);


	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket()-error\n");
		return 0;
	}


	if(bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_un)) == -1){
		perror("bind()-error\n");	
	}

	listen(sockfd, 5);



	epfd = epoll_create1(0);	
	if(epfd == -1){
		perror("epoll_create1()-error\n");
		return 0;
	}


	
	event.events = EPOLLIN;
	event.data.fd = client_sockfd;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event) == -1){
		perror("epoll_ctl()-error\n");
		return 0;
	}
	
	
	while(1){
		
		structfd.sockfd = &sockfd;
		structfd.client_sockfd = &client_sockfd;
		pthread_create(&accept_pt , NULL, accept_thread, (void *)&structfd);


		printf("wait connecting...\n");
		memset(&event, 0, sizeof(event));
		ret = epoll_wait(epfd, &event, 1, 10000);

		if(ret == -1){
			perror("epoll_wait()-error\n");
			return 0;
		}

		else if(ret == 0){
			printf("timeout!\n");
		}

		else if(ret > 0){
			printf("fu\n");		
			memset(buf, 0, sizeof(buf));
			if(recv(client_sockfd, buf, sizeof(buf), 0) == -1){
				perror("recv()-error\n");
				return 0;
			}
			
			if(atoi(buf) == 6){
				memset(buf, 0, sizeof(buf));
				strncpy(buf, "correct", sizeof(buf));
				send(client_sockfd, buf, sizeof(buf), 0);
			}

			else if(atoi(buf) != 6){
				memset(buf, 0, sizeof(buf));
				strncpy(buf, "incorrect", sizeof(buf));
				send(client_sockfd, buf, sizeof(buf), 0);
			}

		}	

		/*
		Client_node *tmp_node = client_head_list->next;
		pthread_create(&(tmp_node->data.pt), NULL, fight_pthread, (void *)&client_sockfd); 
		*/
	}


	return 0;
}
