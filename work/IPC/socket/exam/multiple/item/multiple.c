#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <sys/un.h>

#define PATHNAME	"abcd"


typedef struct Client_info{

	int client_sockfd;

}Client_info;


typedef struct Client_node{

	Client_info data;	
	struct client_node *next;

}Client_node;


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

/* head_list */
Client_node *client_head_list; 


int main(int argc, char **argv)
{
	/* head_list initialize */

	client_head_list = (Client_node *)malloc(sizeof(Client_node));
	client_head_list->next = NULL;

	/* initiallize variable  */

	int sockfd;
	int client_sockfd;
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

	
	while(1){

		printf("wait connecting...\n");

		client_sockfd = accept(sockfd, NULL, NULL);
		if(client_sockfd == -1){
			perror("accept()-error\n");
			return 0;
		}

		printf("client connected\n");

		insert_node(client_head_list, client_sockfd);
		print_node_list(client_head_list);
	}


	return 0;
}
